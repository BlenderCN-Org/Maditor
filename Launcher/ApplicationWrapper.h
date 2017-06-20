#pragma once

#include "Shared/AppControl.h"

#include "App\Ogreappsettings.h"
#include "App/OgreApplication.h"

#include "ModuleLoader.h"
#include "Util\LogListener.h"
#include "Serialize\Container\serialized.h"

#include "Util\Util.h"

#include "App\framelistener.h"

#include "Inspector.h"

namespace Maditor {

	

	namespace Launcher {

		class ApplicationWrapper : public Shared::AppControl, public Engine::App::FrameListener {

		public:
			ApplicationWrapper(size_t id);
			~ApplicationWrapper();

			int start();
			
		protected:
			// Inherited via AppControl
			virtual void shutdownImpl() override;
		
			// Inherited via AppControl
			virtual void onApplicationSetup() override;

			virtual bool frameStarted(float timeSinceLastFrame) override;
			virtual bool frameRenderingQueued(float timeSinceLastFrame) override;
			virtual bool frameEnded(float timeSinceLastFrame) override;

			void startImpl();

			void stopImpl();

			void pauseImpl();

			virtual void resizeWindowImpl() override;

			virtual void execLuaImpl(const std::string &cmd) override;

		private:
			virtual size_t getSize() const override;

			Engine::Serialize::Serialized<LogListener> mLog;
			Engine::Serialize::Serialized<ModuleLoader> mLoader;
			Engine::Serialize::Serialized<Util> mUtil;
			Engine::Serialize::Serialized<Inspector> mInspector;

			Engine::App::OgreAppSettings mSettings;
			Engine::App::OgreApplication *mApplication;
			Engine::Server::ServerBase *mServer;

			InputWrapper *mInput;

			

			bool mRunning;			
			bool mStartRequested;
			
		};

	}
}