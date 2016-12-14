#pragma once

#include "Shared/AppControl.h"

#include "App\appsettings.h"
#include "App/Application.h"

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
			ModuleLoader *mLoader;

			bool mRunning = true;

			
		};

	}
}