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
			void start();
			void pause();
			void stop();
			
			OgreWindow *window();

			void addProcessListener(std::function<void(DWORD)> f);

			DWORD pid();

		protected:
			virtual void timerEvent(QTimerEvent * te) override;

			void shutdown();
			void cleanup();

			// Inherited via AppControl
			virtual void shutdownImpl() override;


			// Inherited via AppControl
			virtual void startImpl() override;

			virtual void stopImpl() override;

			virtual void pauseImpl() override;

			// Inherited via AppControl
			virtual void onApplicationInitialized() override;

		signals:
			void applicationInitializing();
			void applicationInitialized();
			void applicationShutdown();

		private:
			OgreWindow *mWindow;

			QString mPath;

			DWORD mPID;
			HANDLE mHandle;

			Engine::Serialize::Serialized<ModuleLoader> mLoader;
			

			std::list<std::function<void(DWORD)>> mProcessListener;
			


		};

	}
}