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
			ApplicationLauncher(const QString &path, const ModuleList &modules, LogsModel *logs);
			virtual ~ApplicationLauncher();

			void init();
			void initImpl(bool debug);
			void initNoDebug();
			void shutdown();
			void kill();
			void start();
			void pause();
			void stop();
			
			OgreWindow *window();
			ModuleLoader *moduleLoader();
			UtilModel *util();

			void addProcessListener(std::function<void(DWORD, const Shared::ApplicationInfo &)> f);

			DWORD pid();

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
			virtual void onApplicationInitialized() override;

			virtual void pingImpl() override;

		protected slots:
		    void resizeWindow();

		signals:
			void applicationInitializing();
			void applicationInitialized();
			void applicationStarted();
			void applicationStopped();
			void applicationShutdown();

		private:
			std::unique_ptr<InputWrapper> mInput;
			OgreWindow *mWindow;

			QString mPath;

			DWORD mPID;
			HANDLE mHandle;

			Engine::Serialize::Serialized<OgreLogReader> mLog;
			Engine::Serialize::Serialized<ModuleLoader> mLoader;
			UtilModel mUtil;
			
			bool mWaitingForLoader;

			QTimer mPingTimer;

			std::list<std::function<void(DWORD, const Shared::ApplicationInfo &)>> mProcessListener;
			


		};

	}
}