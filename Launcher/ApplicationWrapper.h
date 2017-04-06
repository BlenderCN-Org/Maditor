#pragma once

#include "Shared/AppControl.h"

#include "App\Ogreappsettings.h"
#include "App/OgreApplication.h"

#include "ModuleLoader.h"
#include "Util\LogListener.h"
#include "Serialize\Container\serialized.h"

#include "Util\Util.h"

namespace Maditor {
	namespace Launcher {

		class ApplicationWrapper : public Shared::AppControl, public Ogre::FrameListener {

		public:
			ApplicationWrapper(size_t id);

			int start();
			
		protected:
			// Inherited via AppControl
			virtual void shutdownImpl() override;
		
			// Inherited via AppControl
			virtual void onApplicationSetup() override;

			virtual bool frameStarted(const Ogre::FrameEvent &evt) override;
			virtual bool frameRenderingQueued(const Ogre::FrameEvent &evt) override;
			virtual bool frameEnded(const Ogre::FrameEvent &evt) override;

			void startImpl();

			void stopImpl();

			void pauseImpl();

			virtual void resizeWindowImpl() override;

		private:
			virtual size_t getSize() const override;

			Engine::Serialize::Serialized<LogListener> mLog;
			Engine::Serialize::Serialized<ModuleLoader> mLoader;
			Engine::Serialize::Serialized<Util> mUtil;

			Engine::App::OgreAppSettings mSettings;
			Engine::App::OgreApplication mApplication;

			InputWrapper *mInput;

			

			bool mRunning;			
			bool mStartRequested;
			
		};

	}
}