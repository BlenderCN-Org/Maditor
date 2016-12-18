#pragma once

#include "Shared/AppControl.h"
#include "ModuleLoader.h"
#include "Serialize\Container\serialized.h"

namespace Maditor {
	namespace Model {

		class MADITOR_MODEL_EXPORT ApplicationLauncher : public QObject, public Shared::AppControl {
			Q_OBJECT

		public:
			ApplicationLauncher(const QString &path, const ModuleList &modules);
			virtual ~ApplicationLauncher();

			void init();
			
			OgreWindow *window();

		protected:
			virtual void timerEvent(QTimerEvent * te) override;

			void shutdown();
			void cleanup();

			// Inherited via AppControl
			virtual void shutdownImpl() override;

		signals:
			void applicationInitializing();
			void applicationShutdown();

		private:
			OgreWindow *mWindow;

			QString mPath;

			DWORD mPID;
			HANDLE mHandle;

			Engine::Serialize::Serialized<ModuleLoader> mLoader;
			

		};

	}
}