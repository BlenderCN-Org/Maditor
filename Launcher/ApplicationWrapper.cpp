#include "maditorlauncherlib.h"

#include "ApplicationWrapper.h"

#include "Shared\SharedMemory.h"

#include "InputWrapper.h"

#include "GUI\GUISystem.h"

#include "Network\networkmanager.h"

#include "ModuleLoader.h"

namespace Maditor {
	namespace Launcher {

		ApplicationWrapper::ApplicationWrapper() :
			AppControl(false),
			mInput(new InputWrapper(sharedMemory().mInput)),
			mLoader(new ModuleLoader(this)),
			mRunning(false)
		{

			
		}

		int ApplicationWrapper::init()
		{
			Engine::Network::NetworkManager *net = network();

			net->addTopLevelItem(this);
			net->startServer(1000);
			while (net->clientCount() == 0) {
				net->receiveMessages();
				net->acceptConnections();
			}

			Shared::ApplicationInfo &appInfo = sharedMemory().mAppInfo;

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
				return -1;
			}

			mApplication.init();
			//sendMsg(APP_INITIALIZED);

			//mNetwork.addTopLevelItem(&Engine::Scene::SceneManager::getSingleton());




			//Ogre::Root::getSingleton().addFrameListener(this);


			return 0;
		}

		void ApplicationWrapper::shutdownImpl()
		{
			mRunning = false;
			mApplication.shutdown();
		}

	}
}


