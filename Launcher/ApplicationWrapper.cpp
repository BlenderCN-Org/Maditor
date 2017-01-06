#include "maditorlauncherlib.h"

#include "ApplicationWrapper.h"

#include "Shared\SharedMemory.h"

#include "InputWrapper.h"

#include "GUI\GUISystem.h"

#include "Network\networkmanager.h"

#include "Util\Util.h"

namespace Maditor {
	namespace Launcher {

		ApplicationWrapper::ApplicationWrapper() :
			AppControl(false),
			mInput(new InputWrapper(sharedMemory().mInput)),
			mLoader(this),
			mRunning(false),
			mStartRequested(false)
		{			
		}

		int ApplicationWrapper::init()
		{
			Engine::Network::NetworkManager *net = network();

			net->startServer(1000);
			if (!net->acceptConnection(2000)) {
				net->close();
				return -1;
			}

			Shared::ApplicationInfo &appInfo = sharedMemory().mAppInfo;

			size_t j = 0;
			while (appInfo.mDebugged &&
				!IsDebuggerPresent() &&
				!GetAsyncKeyState(VK_F10))
			{
				::Sleep(100);
				if (++j > 50) {
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

			mInput->setSystem(&Engine::GUI::GUISystem::getSingleton());
			std::string project = appInfo.mProjectDir.c_str();

			mLoader->setup(project + "debug/bin/", project + "debug/runtime/");
			while (mLoader->receiving() && mRunning && net->clientCount() == 1) {
				net->receiveMessages();
			}
			if (net->clientCount() != 1 || !mRunning) {
				//net->close();
				return -1;
			}

			net->addTopLevelItem(&Engine::Util::Util::getSingleton());

			mApplication.init();

			applicationInitialized();

			Ogre::Root::getSingleton().addFrameListener(this);

			while (mRunning) {
				net->receiveMessages();
				if (net->clientCount() != 1) {
					net->removeTopLevelItem(&Engine::Util::Util::getSingleton());
					//net->close();
					return -1;
				}
				if (mStartRequested) {
					mApplication.go();
					mStartRequested = false;
					stop();
				}

			}

			net->removeTopLevelItem(&Engine::Util::Util::getSingleton());

			return 0;
		}

		void ApplicationWrapper::shutdownImpl()
		{
			mRunning = false;
			mApplication.shutdown();
		}

		void ApplicationWrapper::onApplicationInitialized()
		{

		}

		bool ApplicationWrapper::frameRenderingQueued(const Ogre::FrameEvent & evt)
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

	}
}


