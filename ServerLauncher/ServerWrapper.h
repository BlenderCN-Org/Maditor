#pragma once

#include "Shared/ServerControl.h"

#include "ModuleLoader.h"
#include "LogListener.h"
#include "Serialize\Container\serialized.h"

namespace Maditor {
	namespace Launcher {

		class ServerWrapper : public Engine::Hierarchy::HierarchyObject<ServerWrapper>, public Shared::ServerControl {

		public:
			ServerWrapper();

			int init();
			
		protected:
			// Inherited via AppControl
			virtual void shutdownImpl() override;
		
			bool update(float timeSinceLastFrame);

		private:
			Engine::Serialize::Serialized<LogListener> mLog;

			Engine::Serialize::Serialized<ModuleLoader> mLoader;

			bool mRunning;			
			
		};

	}
}