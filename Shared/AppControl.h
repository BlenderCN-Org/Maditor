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


			virtual void onApplicationInitialized() = 0;

			Engine::Serialize::Action<> applicationInitialized;
			Engine::Serialize::Action<> start, stop, pause;

		protected:
			Shared &sharedMemory();

			Engine::Network::NetworkManager *network();

			virtual void shutdownImpl() = 0;
			virtual void startImpl() = 0;
			virtual void stopImpl() = 0;
			virtual void pauseImpl() = 0;

		private:
			Engine::Network::NetworkManager *mNetwork;

			SharedMemory *mMemory;
		};
	}
}

