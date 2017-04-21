#pragma once

#include "Shared/AppControl.h"
#include "ModuleLoader.h"
#include "Serialize\Container\serialized.h"
#include "UtilModel.h"
#include "LogReceiver.h"
#include "Model\Documents\Document.h"
#include "Shared\errorcodes.h"

namespace Maditor {

	//

	namespace Model {

		class MADITOR_MODEL_EXPORT ApplicationLauncher : public Document, public Shared::AppControl {
			Q_OBJECT

		public:
			ApplicationLauncher(ApplicationConfig *config, const QString &uniqueName);
			virtual ~ApplicationLauncher();

			virtual void destroy() override;

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

			bool isClient();
			bool isLauncher();

			void sendCommand(const QString &cmd);

		protected:
			virtual void timerEvent(QTimerEvent * te) override;

			void cleanup();
			void checkProcess();

			// Inherited via AppControl
			virtual void shutdownImpl() override;

			// Inherited via AppControl
			virtual void startImpl() override;

			virtual void stopImpl() override;

			virtual void pauseImpl() override;

			// Inherited via AppControl
			virtual void onApplicationSetup() override;

			virtual void pingImpl() override;

		protected slots:
		    void resizeWindow();
			void timeout();
			void kill(Shared::ErrorCode cause);

		signals:
			void applicationSettingup();
			void applicationSetup();
			void applicationStarted();
			void applicationStopped();
			void applicationShutdown();

			void processStarted(DWORD, const Shared::ApplicationInfo &);

			void destroyApplication(ApplicationLauncher *launcher);

			void outputReceived(const QString &);

		private:

			virtual size_t getSize() const override;

			std::unique_ptr<InputWrapper> mInput;
			OgreWindow *mWindow;

			ApplicationConfig *mConfig;

			DWORD mPID;
			HANDLE mHandle;
			HANDLE mChildOutRead, mChildOutWrite, mChildInRead, mChildInWrite;

			Engine::Serialize::Serialized<LogReceiver> mLog;
			Engine::Serialize::Serialized<ModuleLoader> mLoader;
			Engine::Serialize::Serialized<UtilModel> mUtil;
			
			bool mWaitingForLoader;

			QTimer mPingTimer;

			bool mAboutToBeDestroyed;

			bool mRunning;
			bool mSetup;

			QString mName;

			QString mOutput;

		};

	}
}