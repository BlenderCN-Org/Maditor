#pragma once


#include "Serialize\toplevelserializableunit.h"

#include "Serialize\Container\action.h"

namespace Maditor {
	namespace Shared {

		class MADITOR_SHARED_EXPORT AppControl : public Engine::Serialize::TopLevelSerializableUnit {

		public:
			AppControl(bool isLauncher);
			virtual ~AppControl();


			Engine::Serialize::Action<> shutdown;

		protected:
			Shared &sharedMemory();

			Engine::Network::NetworkManager *network();

			virtual void shutdownImpl() = 0;

		private:
			Engine::Network::NetworkManager *mNetwork;

			SharedMemory *mMemory;
		};
	}
}

