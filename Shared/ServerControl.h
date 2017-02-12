#pragma once


#include "Serialize\toplevelserializableunit.h"

#include "Serialize\Container\action.h"

namespace Maditor {
	namespace Shared {

		class MADITOR_SHARED_EXPORT ServerControl : public Engine::Serialize::TopLevelSerializableUnit {

		public:
			ServerControl(bool isLauncher);
			virtual ~ServerControl();

		protected:			

			Engine::Serialize::Action<Engine::Serialize::ActionPolicy::allowAll> shutdown;			
			Engine::Serialize::Action<Engine::Serialize::ActionPolicy::allowAll> ping;

		protected:
			ServerInfo &sharedMemory();

			Engine::Network::NetworkManager *network();

			virtual void shutdownImpl() = 0;			
			virtual void pingImpl();

		private:
			Engine::Network::NetworkManager *mNetwork;

			ServerSharedMemory *mMemory;
		};
	}
}

