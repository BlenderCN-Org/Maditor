#pragma once


#include "Serialize\toplevelserializableunit.h"

#include "Serialize\Container\action.h"

namespace Maditor {
	namespace Shared {

		class MADITOR_SHARED_EXPORT AppControl : public Engine::Serialize::TopLevelSerializableUnit {

		public:
			AppControl(bool isLauncher);
			virtual ~AppControl();

		protected:

			virtual void onApplicationInitialized() = 0;

			Engine::Serialize::Action<Engine::Serialize::ActionPolicy::allowAll> shutdown;
			Engine::Serialize::Action<Engine::Serialize::ActionPolicy::allowAll> applicationInitialized;
			Engine::Serialize::Action<Engine::Serialize::ActionPolicy::allowAll> start, stop, pause;
			Engine::Serialize::Action<Engine::Serialize::ActionPolicy::standard> resizeWindow;
			Engine::Serialize::Action<Engine::Serialize::ActionPolicy::allowAll> ping;

		protected:
			Shared &sharedMemory();

			Engine::Network::NetworkManager *network();

			virtual void shutdownImpl() = 0;
			virtual void startImpl() = 0;
			virtual void stopImpl() = 0;
			virtual void pauseImpl() = 0;
			virtual void resizeWindowImpl();
			virtual void pingImpl();

		private:
			Engine::Network::NetworkManager *mNetwork;

			SharedMemory *mMemory;
		};
	}
}

