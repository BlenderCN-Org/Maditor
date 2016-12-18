#pragma once

#include "Shared/AppControl.h"

#include "App\appsettings.h"
#include "App/Application.h"

#include "ModuleLoader.h"
#include "Serialize\Container\serialized.h"

namespace Maditor {
	namespace Launcher {

		class ApplicationWrapper : public Shared::AppControl {

		public:
			ApplicationWrapper();

			int init();
			
		protected:
			// Inherited via AppControl
			virtual void shutdownImpl() override;
		
		private:
			Engine::App::AppSettings mSettings;
			Engine::App::Application mApplication;

			InputWrapper *mInput;
			Engine::Serialize::Serialized<ModuleLoader> mLoader;

			bool mRunning;			
		};

	}
}