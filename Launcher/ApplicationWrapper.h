#pragma once

#include "Shared/AppControl.h"

#include "App\Ogreappsettings.h"
#include "App/OgreApplication.h"

#include "ModuleLoader.h"
#include "LogListener.h"
#include "Serialize\Container\serialized.h"

namespace Maditor {
	namespace Launcher {

		class ApplicationWrapper : public Engine::Hierarchy::HierarchyObject<ApplicationWrapper>, public Shared::AppControl, public Ogre::FrameListener {

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

			virtual void resizeWindowImpl() override;

		private:
			Engine::Serialize::Serialized<LogListener> mLog;

			Engine::App::OgreAppSettings mSettings;
			Engine::App::OgreApplication mApplication;

			InputWrapper *mInput;

			Engine::Serialize::Serialized<ModuleLoader> mLoader;

			bool mRunning;			
			bool mStartRequested;
			
		};

	}
}