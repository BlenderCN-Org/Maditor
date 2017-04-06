#pragma once

#include "Shared/AppControl.h"
#include "ModuleLoader.h"
#include "Serialize\Container\serialized.h"
#include "UtilModel.h"
#include "OgreLogReader.h"

namespace Maditor {
	namespace Model {

		class MADITOR_MODEL_EXPORT ApplicationLauncher : public QObject, public Shared::AppControl {
			Q_OBJECT

		public:
			ApplicationLauncher(ApplicationConfig *config);
			virtual ~ApplicationLauncher();

			void destroy();

			void setup();
			void setupImpl(bool debug);
			void setupNoDebug();
			void shutdown();
			void kill();
			void start();
			void pause();
			void stop();
			
			OgreWindow *window();
			ModuleLoader *moduleLoader();
			UtilModel *util();

			DWORD pid();

			bool isRunning();
			bool isLaunched();
			bool isSetup();

		protected:
			virtual void timerEvent(QTimerEvent * te) override;

			void cleanup();

			// Inherited via AppControl
			virtual void shutdownImpl() override;

			// Inherited via AppControl
			virtual void startImpl() override;

			virtual void stopImpl() override;

			virtual void pauseImpl() override;

			// Inherited via AppControl
			virtual void onApplicationSetup() override;

			virtual void pingImpl() override;

			enum KillCause {
				TIMEOUT,
				NO_APPLICATION_NOTIFICATION,
				FAILED_CONNECTION,
				CLEANUP,
				USER_REQUEST
			};

		protected slots:
		    void resizeWindow();
			void timeout();
			void kill(KillCause cause);

		signals:
			void applicationSettingup();
			void applicationSetup();
			void applicationStarted();
			void applicationStopped();
			void applicationShutdown();

			void processStarted(DWORD, const Shared::ApplicationInfo &);

			void destroyApplication(ApplicationLauncher *launcher);

		private:

			virtual size_t getSize() const override;

			std::unique_ptr<InputWrapper> mInput;
			OgreWindow *mWindow;

			ApplicationConfig *mConfig;

			DWORD mPID;
			HANDLE mHandle;

			Engine::Serialize::Serialized<OgreLogReader> mLog;
			Engine::Serialize::Serialized<ModuleLoader> mLoader;
			Engine::Serialize::Serialized<UtilModel> mUtil;
			
			bool mWaitingForLoader;

			QTimer mPingTimer;

			bool mAboutToBeDestroyed;

			bool mRunning;
			bool mSetup;

		};

	}
}