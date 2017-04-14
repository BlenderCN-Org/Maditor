#pragma once


#include "Serialize\toplevelserializableunit.h"

#include "Serialize\Container\action.h"

namespace Maditor {
	namespace Shared {

		class MADITOR_SHARED_EXPORT AppControl : public Engine::Serialize::TopLevelSerializableUnit<AppControl> {

		public:
			AppControl();
			AppControl(size_t id);
			virtual ~AppControl();

		protected:

			size_t appId();

			Shared &sharedMemory();

			BoostIPCManager *network();

			virtual void onApplicationSetup() = 0;
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
			void onApplicationSetup2() {
				onApplicationSetup();
			}

			Engine::Serialize::Action<decltype(&AppControl::shutdownImpl2), &AppControl::shutdownImpl2, Engine::Serialize::ActionPolicy::broadcast> shutdown;
			Engine::Serialize::Action<decltype(&AppControl::onApplicationSetup2), &AppControl::onApplicationSetup2, Engine::Serialize::ActionPolicy::broadcast> applicationSetup;
			Engine::Serialize::Action<decltype(&AppControl::startImpl2), &AppControl::startImpl2, Engine::Serialize::ActionPolicy::broadcast> start;
			Engine::Serialize::Action<decltype(&AppControl::stopImpl2), &AppControl::stopImpl2, Engine::Serialize::ActionPolicy::broadcast> stop;
			Engine::Serialize::Action<decltype(&AppControl::pauseImpl2), &AppControl::pauseImpl2, Engine::Serialize::ActionPolicy::broadcast> pause;
			Engine::Serialize::Action<decltype(&AppControl::resizeWindowImpl2), &AppControl::resizeWindowImpl2, Engine::Serialize::ActionPolicy::broadcast> resizeWindow;
			Engine::Serialize::Action<decltype(&AppControl::pingImpl2), &AppControl::pingImpl2, Engine::Serialize::ActionPolicy::broadcast> ping;
			
		private:
			SharedMemory *mMemory;
			BoostIPCManager *mNetwork;

			
		};


	}
}

