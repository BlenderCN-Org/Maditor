#pragma once

#include "Shared/AppControl.h"

#include "App\appsettings.h"
#include "App/Application.h"

#include "ModuleLoader.h"
#include "Serialize\Container\serialized.h"

namespace Maditor {
	namespace Launcher {

		class ApplicationWrapper : public Shared::AppControl, public Ogre::FrameListener {

		public:
			ApplicationWrapper();

			int init();
			
		protected:
			// Inherited via AppControl
			virtual void shutdownImpl() override;
		
			// Inherited via AppControl
			virtual void onApplicationInitialized() override;

			virtual bool frameRenderingQueued(const Ogre::FrameEvent &evt) override;

			void startImpl();

			void stopImpl();

			void pauseImpl();

		private:
			Engine::App::AppSettings mSettings;
			Engine::App::Application mApplication;

			InputWrapper *mInput;
			Engine::Serialize::Serialized<ModuleLoader> mLoader;

			bool mRunning;			

			
		};

	}
}