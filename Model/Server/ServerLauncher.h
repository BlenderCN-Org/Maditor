#pragma once

#include "Shared/ServerControl.h"
#include "Model\Application\ModuleLoader.h"
#include "Serialize\Container\serialized.h"
#include "Model\Application\UtilModel.h"
#include "Model\Application\OgreLogReader.h"
#include "Model\Logs\LogsModel.h"

namespace Maditor {
	namespace Model {

		class MADITOR_MODEL_EXPORT ServerLauncher : public QObject, public Shared::ServerControl {
			Q_OBJECT

		public:
			ServerLauncher(Generators::ServerClassGenerator *serverClass, const QString &path, const ModuleList &modules);
			virtual ~ServerLauncher();

			void init();
			void initImpl(bool debug);
			void initNoDebug();
			void shutdown();
			void kill();
			
			ModuleLoader *moduleLoader();
			UtilModel *util();
			LogsModel *logs();

			DWORD pid();

			QString name();

		protected:
			virtual void timerEvent(QTimerEvent * te) override;

			void cleanup();

			// Inherited via AppControl
			virtual void shutdownImpl() override;

			virtual void pingImpl() override;

		signals:
			void serverInitialized();
			void serverShutdown();
			
			void processStarted(DWORD, const Shared::ServerInfo &);

		private:			
			QString mPath;

			DWORD mPID;
			HANDLE mHandle;

			Engine::Serialize::Serialized<OgreLogReader> mLog;
			Engine::Serialize::Serialized<ModuleLoader> mLoader;
			UtilModel mUtil;
			
			bool mWaitingForLoader;

			QTimer mPingTimer;		

			Generators::ServerClassGenerator *mServerClass;

			LogsModel mLogs;

		};

	}
}