#pragma once

#include "ApplicationLauncher.h"

#include "Shared/IPCManager/boostIPCmanager.h"

namespace Maditor
{
	namespace Model
	{
	
		class MADITOR_MODEL_EXPORT EmbeddedLauncher : public ApplicationLauncher
		{
			Q_OBJECT
		public:
			EmbeddedLauncher(ApplicationConfig *config, const QString &uniqueName);
			~EmbeddedLauncher();

			bool isRemote() override;

			void setupNoDebug();
			

			bool isLaunched();

			std::string runtimeDir();

			OgreWindow *window();

			DWORD pid();

			void sendCommand(const QString &cmd);

		protected:
			void onDisconnected() override;
			void checkProcess();

			// Inherited via AppControl
			void shutdownImpl() override;

			void timerEvent(QTimerEvent *te) override;

			bool setupImpl() override;
			bool setupImpl(bool debug);

			Engine::Serialize::SerializeManager* network() override;

		protected slots:
		    void resizeWindow();
			void killImpl(Shared::ErrorCode cause) override;


		signals:
			void processStarted(DWORD, const Shared::ApplicationInfo &);

		
		private:
			DWORD mPID;
			HANDLE mHandle;
			HANDLE mChildOutRead, mChildOutWrite, mChildInRead, mChildInWrite;


			Shared::SharedMemory mMemory;
			Shared::BoostIPCManager mNetwork;
			std::unique_ptr<InputWrapper> mInput;
			OgreWindow *mWindow;

		};

	}
}