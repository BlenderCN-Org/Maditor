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

			Shared &sharedMemory();

			Engine::Network::NetworkManager *network();

			virtual void onApplicationInitialized() = 0;
			virtual void shutdownImpl() = 0;
			virtual void startImpl() = 0;
			virtual void stopImpl() = 0;
			virtual void pauseImpl() = 0;
			virtual void resizeWindowImpl();
			virtual void pingImpl();

			void shutdownImpl2() {
				shutdownImpl();
			};
			void startImpl2() {
				startImpl();
			}
			void stopImpl2() {
				stopImpl();
			}
			void pauseImpl2() {
				pauseImpl();
			}
			void resizeWindowImpl2() {
				resizeWindowImpl();
			}
			void pingImpl2() {
				pingImpl();
			}
			void onApplicationInitialized2() {
				onApplicationInitialized();
			}

			Engine::Serialize::Action<decltype(&AppControl::shutdownImpl2), &AppControl::shutdownImpl2, Engine::Serialize::ActionPolicy::broadcast> shutdown;
			Engine::Serialize::Action<decltype(&AppControl::onApplicationInitialized2), &AppControl::onApplicationInitialized2, Engine::Serialize::ActionPolicy::broadcast> applicationInitialized;
			Engine::Serialize::Action<decltype(&AppControl::startImpl2), &AppControl::startImpl2, Engine::Serialize::ActionPolicy::broadcast> start;
			Engine::Serialize::Action<decltype(&AppControl::stopImpl2), &AppControl::stopImpl2, Engine::Serialize::ActionPolicy::broadcast> stop;
			Engine::Serialize::Action<decltype(&AppControl::pauseImpl2), &AppControl::pauseImpl2, Engine::Serialize::ActionPolicy::broadcast> pause;
			Engine::Serialize::Action<decltype(&AppControl::resizeWindowImpl2), &AppControl::resizeWindowImpl2, Engine::Serialize::ActionPolicy::broadcast> resizeWindow;
			Engine::Serialize::Action<decltype(&AppControl::pingImpl2), &AppControl::pingImpl2, Engine::Serialize::ActionPolicy::broadcast> ping;
			
		private:
			Engine::Network::NetworkManager *mNetwork;

			SharedMemory *mMemory;
		};
	}
}

