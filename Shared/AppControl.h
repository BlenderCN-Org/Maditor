#pragma once


#include "Serialize\toplevelserializableunit.h"

#include "Serialize\Container\action.h"

namespace Maditor {
	namespace Shared {

		class MADITOR_SHARED_EXPORT AppControlBase : public Engine::Serialize::TopLevelSerializableUnitBase {

		public:
			AppControlBase(bool isLauncher);
			virtual ~AppControlBase();

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

			Engine::Serialize::Action<decltype(&AppControlBase::shutdownImpl2), &AppControlBase::shutdownImpl2, Engine::Serialize::ActionPolicy::broadcast> shutdown;
			Engine::Serialize::Action<decltype(&AppControlBase::onApplicationInitialized2), &AppControlBase::onApplicationInitialized2, Engine::Serialize::ActionPolicy::broadcast> applicationInitialized;
			Engine::Serialize::Action<decltype(&AppControlBase::startImpl2), &AppControlBase::startImpl2, Engine::Serialize::ActionPolicy::broadcast> start;
			Engine::Serialize::Action<decltype(&AppControlBase::stopImpl2), &AppControlBase::stopImpl2, Engine::Serialize::ActionPolicy::broadcast> stop;
			Engine::Serialize::Action<decltype(&AppControlBase::pauseImpl2), &AppControlBase::pauseImpl2, Engine::Serialize::ActionPolicy::broadcast> pause;
			Engine::Serialize::Action<decltype(&AppControlBase::resizeWindowImpl2), &AppControlBase::resizeWindowImpl2, Engine::Serialize::ActionPolicy::broadcast> resizeWindow;
			Engine::Serialize::Action<decltype(&AppControlBase::pingImpl2), &AppControlBase::pingImpl2, Engine::Serialize::ActionPolicy::broadcast> ping;
			
		private:
			Engine::Network::NetworkManager *mNetwork;

			SharedMemory *mMemory;
		};

		template <class T>
		class AppControl : public Engine::Hierarchy::HierarchyObject<T>, public AppControlBase {
			using AppControlBase::AppControlBase;
		};

	}
}

