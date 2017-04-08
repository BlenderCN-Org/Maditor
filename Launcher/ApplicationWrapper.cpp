#include "maditorlauncherlib.h"

#include "ApplicationWrapper.h"

#include "Shared\SharedMemory.h"

#include "InputWrapper.h"

#include "GUI\GUISystem.h"

#include "Network\networkmanager.h"

#include "Server\serverbase.h"


namespace Maditor {
	namespace Launcher {

		ApplicationWrapper::ApplicationWrapper(size_t id) :
			AppControl(id),
			mInput(nullptr),
			mRunning(false),
			mStartRequested(false),
			mUtil(&mApplication)
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
			
			Engine::Network::NetworkManager *net = network();

			net->startServer(1000);
			if (!net->acceptConnection(2000)) {
				net->close();
				return -1;
			}			

			size_t j = 0;
			while (appInfo.mDebugged &&
				!IsDebuggerPresent() &&
				!GetAsyncKeyState(VK_F10))
			{
				::Sleep(1);
				if (++j > 5000) {
					return -1;
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

				mApplication.setup(mSettings);
				mUtil->setup();
			}

			mRunning = true;			

			Engine::Util::UtilMethods::addListener(mLog.ptr());

			/*Ogre::LogManager::getSingleton().getLog("Ogre.log")->addListener(mLog.ptr());*/

			std::string project = appInfo.mProjectDir.c_str();

			mLoader->setup(project + "debug/bin/", project + "debug/runtime/");
			while (mLoader->receiving() && mRunning && net->clientCount() == 1) {
				net->receiveMessages();
			}
			if (net->clientCount() != 1 || !mRunning) {
				mUtil->shutdown();
				//net->close();
				return -1;
			}

			Engine::Server::ServerBase *server;
			int result = 0;
			if (appInfo.mServerClass.empty()) {
				Ogre::Root::getSingleton().addFrameListener(this);
				mInput->setSystem(&Engine::GUI::GUISystem::getSingleton());
				if (!mApplication.init()) {
					mUtil->shutdown();
					return -1;
				}
				applicationSetup({});

				while (mRunning) {
					net->receiveMessages();
					if (net->clientCount() != 1) {
						mUtil->shutdown();
						//net->close();
						return -1;
					}
					if (mStartRequested) {
						mApplication.go();
						mStartRequested = false;
						stop({});
					}

				}
				mUtil->shutdown();
			}
			else {
				server = mLoader->createServer(appInfo.mServerClass.c_str(), appInfo.mMediaDir.c_str());
				if (!server)
					return -1;
				server->addFrameCallback([this](float timeSinceLastFrame) {return update(); });
				applicationSetup({});
				result = server->run();
				delete server;
			}		

			
			return result;
		}

		void ApplicationWrapper::shutdownImpl()
		{
			mRunning = false;
			mApplication.shutdown();
		}

		void ApplicationWrapper::onApplicationSetup()
		{

		}

		bool ApplicationWrapper::frameRenderingQueued(const Ogre::FrameEvent & evt)
		{
			mUtil->profiler()->stopProfiling(); // PreRender

			mUtil->profiler()->startProfiling("Rendering");

			return update();
		}

			


		bool ApplicationWrapper::frameStarted(const Ogre::FrameEvent & fe)
		{	

			mUtil->update();
			mUtil->profiler()->startProfiling("Frame");
			mUtil->profiler()->startProfiling("PreRender");

			return true;
		}

		bool ApplicationWrapper::frameEnded(const Ogre::FrameEvent & fe)
		{
			mUtil->profiler()->stopProfiling(); // Rendering
			mUtil->profiler()->stopProfiling(); // Frame

			return true;
		}

		bool ApplicationWrapper::update()
		{
			network()->receiveMessages();
			if (network()->clientCount() != 1) {
				shutdownImpl();
			}
			return mRunning;
		}


		void ApplicationWrapper::startImpl()
		{
			mStartRequested = true;
		}
		void ApplicationWrapper::stopImpl()
		{
			mApplication.shutdown();
		}
		void ApplicationWrapper::pauseImpl()
		{

		}

		void ApplicationWrapper::resizeWindowImpl()
		{
			mApplication.resizeWindow();
		}

		size_t ApplicationWrapper::getSize() const
		{
			return sizeof(ApplicationWrapper);
		}

	}
}


