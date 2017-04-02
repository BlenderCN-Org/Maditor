#pragma once


#include "Serialize\toplevelserializableunit.h"

#include "Serialize\Container\action.h"

namespace Maditor {
	namespace Shared {

		class MADITOR_SHARED_EXPORT ServerControl : public Engine::Serialize::TopLevelSerializableUnit<ServerControl> {

		public:
			ServerControl(bool isLauncher);
			virtual ~ServerControl();
			

		protected:
			ServerInfo &sharedMemory();

			Engine::Network::NetworkManager *network();

			virtual void shutdownImpl() = 0;			
			virtual void pingImpl();

			void shutdownImpl2() {
				shutdownImpl();
			}
			void pingImpl2() {
				pingImpl();
			}

			Engine::Serialize::Action<decltype(&ServerControl::shutdownImpl2), &ServerControl::shutdownImpl2, Engine::Serialize::ActionPolicy::broadcast> shutdown;
			Engine::Serialize::Action<decltype(&ServerControl::pingImpl2), &ServerControl::pingImpl2, Engine::Serialize::ActionPolicy::broadcast> ping;
			
		private:
			Engine::Network::NetworkManager *mNetwork;

			ServerSharedMemory *mMemory;
		};
	}
}

