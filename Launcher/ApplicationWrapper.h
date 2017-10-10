#pragma once

#include "Shared/AppControl.h"

#ifdef MADGINE_CLIENT_BUILD
#include "App\Ogreappsettings.h"
#include "App/OgreApplication.h"
#else
#include "App/serverappsettings.h"
#endif

#include "ModuleLoader.h"
#include "Util\LogListener.h"
#include "Serialize\Container\serialized.h"

#include "Util\Util.h"

#include "App\framelistener.h"

#include "Inspector.h"

#include "Shared/SharedMemory.h"
#include "Shared/IPCManager/boostIPCmanager.h"

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

#ifdef MADGINE_CLIENT_BUILD
			Engine::App::OgreAppSettings mSettings;
			std::unique_ptr<Engine::App::OgreApplication> mApplication;

			InputWrapper *mInput;
#else
			Engine::App::ServerAppSettings mSettings;
			std::unique_ptr<Engine::Server::ServerBase> mServer;
#endif			

			bool mRunning;			
			bool mStartRequested;
			
			Shared::SharedMemory mMemory;
			Shared::BoostIPCManager mNetwork;

		};

	}
}