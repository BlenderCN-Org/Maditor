#include "maditorlauncherlib.h"

#include "ApplicationWrapper.h"

#include "Shared\SharedMemory.h"



#include "Shared\IPCManager\boostIPCmanager.h"

#ifdef MADGINE_SERVER_BUILD
#include "Server\serverbase.h"
#else
#include "InputWrapper.h"
#include "gui/guisystem.h"
#endif

#include "Shared\errorcodes.h"


#include <iostream>

namespace Maditor {
	namespace Launcher {

		ApplicationWrapper::ApplicationWrapper(size_t id) :
			AppControl(Shared::AppControl::masterLauncher),
			mMemory(id),
			mNetwork(&mMemory, "Maditor-Link"),
			mInput(nullptr),
			mRunning(false),
			mStartRequested(false)
		{
			Engine::Serialize::Debugging::StreamDebugging::setLoggingEnabled(true);

			mNetwork.addTopLevelItem(this);

			init();
		}

		ApplicationWrapper::~ApplicationWrapper()
		{
			Engine::SignalSlot::ConnectionManager::getSingleton().update();
			mNetwork.sendMessages();
			Engine::SignalSlot::ConnectionStore::globalStore().clear();
			using namespace std::chrono;
			std::this_thread::sleep_for(2000ms);
			if (mInput)
				delete mInput;
		}


		int ApplicationWrapper::start()
		{

			Shared::ApplicationInfo &appInfo = mMemory.data().mAppInfo;

			Shared::BoostIPCManager *net = &mNetwork;

			if (!net->startServer()) {
				return Shared::FAILED_START_SERVER;
			}
			if (!net->acceptConnection(2000)) {
				net->close();
				return Shared::MADITOR_CONNECTION_TIMEOUT;
			}

			size_t j = 0;
			while (appInfo.mDebugged &&
				!IsDebuggerPresent() &&
				!GetAsyncKeyState(VK_F10))
			{
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
				if (++j > 5000) {
					return Shared::DEBUGGER_CONNECTION_TIMEOUT;
				}
			}

			int result = 0;


			mRunning = true;

			std::string project = appInfo.mProjectDir.c_str();
			mLoader->setup(project + "debug/bin/");
			while (mLoader->receiving() && mRunning && net->clientCount() == 1) {
				net->sendAndReceiveMessages();
			}
			if (net->clientCount() != 1 || !mRunning) {
				//net->close();
				return Shared::MODULE_LOAD_FAILED;
			}

#ifdef MADGINE_CLIENT_BUILD

			assert(appInfo.mServerClass.empty());

			mSettings.mRootDir = appInfo.mMediaDir.c_str();
			mSettings.mPluginsFile = mSettings.mRootDir + "plugins.cfg";

			mInput = new InputWrapper(mMemory.data().mInput);
			mSettings.mInput = mInput;
			mSettings.mUseExternalSettings = true;
			mSettings.mWindowName = "QtOgre";
			mSettings.mWindowWidth = appInfo.mWindowWidth;
			mSettings.mWindowHeight = appInfo.mWindowHeight;
			mSettings.mAppName = appInfo.mAppName.c_str();

			Ogre::NameValuePairList &parameters = mSettings.mWindowParameters;

			/*
			Flag within the parameters set so that Ogre3D initializes an OpenGL context on it's own.
			*/
			parameters["currentGLContext"] = Ogre::String("false");

			/*
			We need to supply the low level OS window handle to this QWindow so that Ogre3D knows where to draw
			the scene. Below is a cross-platform method on how to do this.
			If you set both options (externalWindowHandle and parentWindowHandle) this code will work with OpenGL
			and DirectX.
			*/
#if defined(Q_OS_MAC) || defined(Q_OS_WIN)
			parameters["externalWindowHandle"] = Ogre::StringConverter::toString();
			parameters["parentWindowHandle"] = Ogre::StringConverter::toString((size_t)(target->winId()));
#else
			parameters["externalWindowHandle"] = Ogre::StringConverter::toString(appInfo.mWindowHandle);
			parameters["parentWindowHandle"] = Ogre::StringConverter::toString(appInfo.mWindowHandle);
#endif

#if defined(Q_OS_MAC)
			parameters["macAPI"] = "cocoa";
			parameters["macAPICocoaUseNSView"] = "true";
#endif

			mApplication = std::make_unique<Engine::App::OgreApplication>();
			mApplication->setup(mSettings);
			mUtil->setApp(mApplication.get());

			mLog->init();

			Engine::Util::UtilMethods::addListener(mLog.ptr());




			Ogre::LogManager::getSingleton().getLog("Ogre.log")->addListener(mLog.ptr());
			mApplication->addFrameListener(this);
			mInput->setSystem(&Engine::GUI::GUISystem::getSingleton());
			if (!mApplication->init()) {
				mApplication->finalize();
				return Shared::APP_INIT_FAILED;
			}
			applicationSetup({});

			while (mRunning) {
				net->sendAndReceiveMessages();
				Engine::SignalSlot::ConnectionManager::getSingleton().update();
				if (net->clientCount() != 1) {
					//net->close();
					return Shared::MADITOR_DISCONNECTED;
				}
				if (mStartRequested) {
					mApplication->go();
					mStartRequested = false;
					stop({});
				}
			}
			mApplication->finalize();

#else

			assert(!appInfo.mServerClass.empty());
			mServer = std::unique_ptr<Engine::Server::ServerBase>(mLoader->createServer(appInfo.mServerClass.c_str(), appInfo.mAppName.c_str(), appInfo.mMediaDir.c_str()));
			if (!mServer)
				return Shared::FAILED_CREATE_SERVER_CLASS;

			mLog->init();

			Engine::Util::UtilMethods::addListener(mLog.ptr());

			mServer->addFrameListener(this);
			applicationSetup({});
			result = mServer->run();
			mRunning = false;


#endif					
			return result;
		}

		void ApplicationWrapper::shutdownImpl()
		{
			mRunning = false;
			stopImpl();
		}

		void ApplicationWrapper::onApplicationSetup()
		{
			mInspector->init();
		}

		bool ApplicationWrapper::frameRenderingQueued(float timeSinceLastFrame)
		{
			mUtil->profiler()->stopProfiling(); // PreRender

			mUtil->profiler()->startProfiling("Rendering");

			mNetwork.sendAndReceiveMessages();
			if (!mNetwork.isMaster()) {
				shutdownImpl();
			}
			return mRunning;
		}

			


		bool ApplicationWrapper::frameStarted(float timeSinceLastFrame)
		{	

			mUtil->update();
			mUtil->profiler()->startProfiling("Frame");
			mUtil->profiler()->startProfiling("PreRender");

			return true;
		}

		bool ApplicationWrapper::frameEnded(float timeSinceLastFrame)
		{
			mUtil->profiler()->stopProfiling(); // Rendering
			mUtil->profiler()->stopProfiling(); // Frame

			return true;
		}


		void ApplicationWrapper::startImpl()
		{
			mStartRequested = true;
		}
		void ApplicationWrapper::stopImpl()
		{
#ifdef MADGINE_CLIENT_BUILD
				mApplication->shutdown();
#else
				mServer->shutdown();
#endif
		}
		void ApplicationWrapper::pauseImpl()
		{

		}

		void ApplicationWrapper::resizeWindowImpl()
		{
#ifdef MADGINE_CLIENT_BUILD
				mApplication->resizeWindow();
#endif
		}

		void ApplicationWrapper::execLuaImpl(const std::string & cmd)
		{
			std::cout << cmd << std::endl;
			Engine::Scripting::GlobalScopeBase *scope;
#ifdef MADGINE_CLIENT_BUILD
			scope = mApplication.get();
#else
			scope = mServer.get();
#endif
			scope->executeString(cmd);
			std::cout.flush();
		}

		size_t ApplicationWrapper::getSize() const
		{
			return sizeof(ApplicationWrapper);
		}

	}
}


