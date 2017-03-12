#include "maditorserverlauncherlib.h"
#include "serverlib.h"

#include "ServerWrapper.h"

#include "Shared\SharedMemory.h"

#include "GUI\GUISystem.h"

#include "Network\networkmanager.h"

#include "Util\Util.h"

#include "Shared\ServerInfo.h"

#include "Server/baseserver.h"

namespace Maditor {
	namespace Launcher {

		ServerWrapper::ServerWrapper() :
			ServerControl(false),
			mLoader(),
			mRunning(false),
			mLog()
		{			
		}

		int ServerWrapper::init()
		{
			Engine::Network::NetworkManager *net = network();

			net->startServer(1001);
			if (!net->acceptConnection(2000)) {
				net->close();
				return -1;
			}

			Shared::ServerInfo &serverInfo = sharedMemory();

			size_t j = 0;
			while (serverInfo.mDebugged &&
				!IsDebuggerPresent() &&
				!GetAsyncKeyState(VK_F10))
			{
				::Sleep(100);
				if (++j > 50) {
					return -1;
				}
			}

			Util::UtilMethods::addListener(mLog.ptr());

			mRunning = true;

			std::string project = serverInfo.mProjectDir.c_str();

			mLoader->setup(project + "debug/bin/", project + "debug/runtime/");
			while (mLoader->receiving() && mRunning && net->clientCount() == 1) {
				net->receiveMessages();
			}
			if (net->clientCount() != 1 || !mRunning) {
				//net->close();
				return -1;
			}

			Engine::Server::BaseServer *server = mLoader->createServer(serverInfo.mServerClass.c_str(), serverInfo.mMediaDir.c_str());

			if (!server)
				return -1;

			server->addFrameCallback([this](float timeSinceLastFrame) {return update(timeSinceLastFrame); });

			int result = server->run();

				/*net->addTopLevelItem(&Engine::Util::Util::getSingleton());

				mApplication.init();

				applicationInitialized();

				Ogre::Root::getSingleton().addFrameListener(this);*/

				//Run


				//net->removeTopLevelItem(&Engine::Util::Util::getSingleton());

			delete server;

			return result;
		}

		void ServerWrapper::shutdownImpl()
		{
			mRunning = false;
		}

		bool ServerWrapper::update(float timeSinceLastFrame)
		{
			network()->receiveMessages();
			if (network()->clientCount() != 1) {
				shutdownImpl();
			}
			return mRunning;
		}


	}
}


