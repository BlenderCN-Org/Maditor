#include "maditorlauncherlib.h"

#include "ApplicationWrapper.h"

#include "Shared\SharedMemory.h"

#include "InputWrapper.h"

#include "GUI\GUISystem.h"

#include "Network\networkmanager.h"



namespace Maditor {
	namespace Launcher {

		ApplicationWrapper::ApplicationWrapper(size_t id) :
			AppControl(id),
			mInput(new InputWrapper(sharedMemory().mInput)),
			mRunning(false),
			mStartRequested(false),
			mUtil(&mApplication)
		{
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
			
			mSettings.mInput = mInput;
			mSettings.mUseExternalSettings = true;
			mSettings.mWindowName = "QtOgre";
			mSettings.mWindowWidth = appInfo.mWindowWidth;
			mSettings.mWindowHeight = appInfo.mWindowHeight;
			mSettings.mRootDir = appInfo.mMediaDir.c_str();
			mSettings.mPluginsFile = mSettings.mRootDir + "plugins.cfg";

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

			mRunning = true;

			mApplication.setup(mSettings);
			mUtil->setup();

			Engine::Util::UtilMethods::addListener(mLog.ptr());

			/*Ogre::LogManager::getSingleton().getLog("Madgine.log")->addListener(mLog.ptr());
			Ogre::LogManager::getSingleton().getLog("Ogre.log")->addListener(mLog.ptr());*/

			mInput->setSystem(&Engine::GUI::GUISystem::getSingleton());
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

			if (!mApplication.init()) {
				mUtil->shutdown();
				return -1;
			}

					
			applicationSetup({});

			Ogre::Root::getSingleton().addFrameListener(this);

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
			return 0;
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


			network()->receiveMessages();
			if (network()->clientCount() != 1) {
				shutdownImpl();
			}
			return mRunning;
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


