#include "maditorlauncherlib.h"

#include "ApplicationWrapper.h"

#include "Shared\SharedMemory.h"

#include "InputWrapper.h"

#include "GUI\GUISystem.h"

#include "Shared\IPCManager\boostIPCmanager.h"

#include "Server\serverbase.h"

#include "Shared\errorcodes.h"

#include <iostream>

namespace Maditor {
	namespace Launcher {

		ApplicationWrapper::ApplicationWrapper(size_t id) :
			AppControl(id),
			mInput(nullptr),
			mRunning(false),
			mStartRequested(false),
			mApplication(nullptr),
			mServer(nullptr)
		{
		}

		ApplicationWrapper::~ApplicationWrapper()
		{
			if (mInput)
				delete mInput;
		}


		int ApplicationWrapper::start()
		{

			Shared::ApplicationInfo &appInfo = sharedMemory().mAppInfo;
			appInfo.setAppId(masterId());	

			Shared::BoostIPCManager *net = network();

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
				::Sleep(1);
				if (++j > 5000) {
					return Shared::DEBUGGER_CONNECTION_TIMEOUT;
				}
			}

			mSettings.mRootDir = appInfo.mMediaDir.c_str();
			mSettings.mPluginsFile = mSettings.mRootDir + "plugins.cfg";

			if (appInfo.mServerClass.empty()) {

				mInput = new InputWrapper(sharedMemory().mInput);
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

				mApplication = new Engine::App::OgreApplication;
				mApplication->setup(mSettings);
				mUtil->setApp(mApplication);

				Ogre::LogManager::getSingleton().getLog("Ogre.log")->addListener(mLog.ptr());
			}

			mRunning = true;			

			Engine::Util::UtilMethods::addListener(mLog.ptr());
					

			std::string project = appInfo.mProjectDir.c_str();

			mLoader->setup(project + "debug/bin/", project + "debug/runtime/");
			while (mLoader->receiving() && mRunning && net->clientCount() == 1) {
				net->sendAndReceiveMessages();
			}
			if (net->clientCount() != 1 || !mRunning) {
				//net->close();
				return Shared::MODULE_LOAD_FAILED;
			}

			
			int result = 0;
			if (appInfo.mServerClass.empty()) {
				mApplication->addFrameListener(this);
				mInput->setSystem(&Engine::GUI::GUISystem::getSingleton());
				if (!mApplication->init()) {
					return Shared::APP_INIT_FAILED;
				}
				applicationSetup({});

				while (mRunning) {
					net->sendAndReceiveMessages();
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
				delete mApplication;
				mApplication = nullptr;
			}
			else {
				mServer = mLoader->createServer(appInfo.mServerClass.c_str(), appInfo.mAppName.c_str(), appInfo.mMediaDir.c_str());
				if (!mServer)
					return Shared::FAILED_CREATE_SERVER_CLASS;
				mServer->addFrameListener(this);
				applicationSetup({});
				result = mServer->run();
				delete mServer;
				mServer = nullptr;
				mRunning = false;
			}		

			
			return result;
		}

		void ApplicationWrapper::shutdownImpl()
		{
			mRunning = false;
			stopImpl();
		}

		void ApplicationWrapper::onApplicationSetup()
		{

		}

		bool ApplicationWrapper::frameRenderingQueued(float timeSinceLastFrame)
		{
			mUtil->profiler()->stopProfiling(); // PreRender

			mUtil->profiler()->startProfiling("Rendering");

			network()->sendAndReceiveMessages();
			if (network()->clientCount() != 1) {
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
			if (mApplication)
				mApplication->shutdown();
			if (mServer)
				mServer->shutdown();
		}
		void ApplicationWrapper::pauseImpl()
		{

		}

		void ApplicationWrapper::resizeWindowImpl()
		{
			mApplication->resizeWindow();
		}

		void ApplicationWrapper::execLuaImpl(const std::string & cmd)
		{
			std::cout << cmd << std::endl;
			Engine::Scripting::Parsing::ScriptParser::getSingleton().executeString(cmd);
			std::cout.flush();
		}

		size_t ApplicationWrapper::getSize() const
		{
			return sizeof(ApplicationWrapper);
		}

	}
}


