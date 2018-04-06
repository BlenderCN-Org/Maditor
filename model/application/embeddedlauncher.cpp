#include "maditormodellib.h"

#include "EmbeddedLauncher.h"

#include "InputWrapper.h"

#include "Project\ApplicationConfig.h"

#include "Project\Project.h"

#include "OgreWindow.h"

#include "Shared\IPCManager\boostIPCServer.h"

namespace Maditor
{
	namespace Model
	{
		EmbeddedLauncher::EmbeddedLauncher(ApplicationConfig* config, const QString& uniqueName) :
			ApplicationLauncher(config, uniqueName),
			mNetwork(&mMemory, uniqueName.toStdString()),
			mInput(new InputWrapper(mMemory.data().mInput)),
			mWindow(config->launcher() == ApplicationConfig::MADITOR_LAUNCHER ? new OgreWindow(mInput.get()) : nullptr),
			mPID(0),
			mChildInRead(NULL),
			mChildInWrite(NULL),
			mChildOutRead(NULL),
			mChildOutWrite(NULL)
		{
			mNetwork.connectConnectionSlot(mOnSetupResult);

			mNetwork.addTopLevelItem(this);

			if (mWindow)
				connect(mWindow, &OgreWindow::resized, this, &EmbeddedLauncher::resizeWindow);

			SECURITY_ATTRIBUTES sa;

			sa.nLength = sizeof(sa);
			sa.bInheritHandle = true;
			sa.lpSecurityDescriptor = NULL;

			if (!CreatePipe(&mChildOutRead, &mChildOutWrite, &sa, 0))
				return;

			if (!SetHandleInformation(mChildOutRead, HANDLE_FLAG_INHERIT, 0))
				return;

			if (!CreatePipe(&mChildInRead, &mChildInWrite, &sa, 0))
				return;

			if (!SetHandleInformation(mChildInWrite, HANDLE_FLAG_INHERIT, 0))
				return;

			std::experimental::filesystem::create_directory(runtimeDir());
		}

		EmbeddedLauncher::~EmbeddedLauncher()
		{
			killImpl(Shared::KILL_CLEANUP);

			if (mChildInRead)
			{
				CloseHandle(mChildInRead);
				mChildInRead = NULL;
			}
			if (mChildInWrite)
			{
				CloseHandle(mChildInWrite);
				mChildInWrite = NULL;
			}
			if (mChildOutRead)
			{
				CloseHandle(mChildOutRead);
				mChildOutRead = NULL;
			}
			if (mChildOutWrite)
			{
				CloseHandle(mChildOutWrite);
				mChildOutWrite = NULL;
			}
		}


		void EmbeddedLauncher::setupNoDebug()
		{
			setupImpl(false);
		}

		bool EmbeddedLauncher::setupImpl(bool debug)
		{
			mAppInfo.mDebugged = debug;
			mAppInfo.mAppName = getName().toStdString();

			std::string cmd;


			if (isLauncher())
			{
				mConfig->generateInfo(mAppInfo, mWindow);

				cmd = isClient() ? "Embedded_Client_Launcher.exe " : "Embedded_Server_Launcher.exe ";

				cmd += std::to_string(mMemory.id());
			}
			else
			{
				cmd = mConfig->customExecutableCmd().toStdString();
			}

			STARTUPINFO si;
			PROCESS_INFORMATION pi;

			// set the size of the structures
			ZeroMemory(&si, sizeof(si));
			si.cb = sizeof(si);
			si.hStdError = mChildOutWrite;
			si.hStdOutput = mChildOutWrite;
			si.hStdInput = mChildInRead;
			si.dwFlags |= STARTF_USESTDHANDLES;

			ZeroMemory(&pi, sizeof(pi));

			// start the program up
			bool result = CreateProcess(NULL, // the path
			                            const_cast<char*>(cmd.c_str()),
			                            NULL, // Process handle not inheritable
			                            NULL, // Thread handle not inheritable
			                            TRUE, // Set handle inheritance to TRUE
			                            0, // No creation flags
			                            NULL, // Use parent's environment block
			                            mAppInfo.mProjectDir.empty() ? NULL : runtimeDir().c_str(),
			                            &si, // Pointer to STARTUPINFO structure
			                            &pi // Pointer to PROCESS_INFORMATION structure
			);

			if (!result)
				return false;

			mPID = pi.dwProcessId;
			mHandle = pi.hProcess;
			CloseHandle(pi.hThread);

			emit processStarted(mPID, mAppInfo);
			mUtil->stats()->setProcess(mHandle);

			if (isLauncher())
			{
				mNetwork.connect_async(10000);
			}
			else
			{
				onApplicationConnected();
			}

			return true;
			
		}


		bool EmbeddedLauncher::isRemote()
		{
			return false;
		}

		std::string EmbeddedLauncher::runtimeDir()
		{
			return (mConfig->project()->path() + "debug/runtime/" + getName() + "/").toStdString();
		}

		
		OgreWindow* EmbeddedLauncher::window()
		{
			return mWindow;
		}

		
		DWORD EmbeddedLauncher::pid()
		{
			return mPID;
		}

		bool EmbeddedLauncher::isLaunched()
		{
			return mPID != 0;
		}

		void EmbeddedLauncher::resizeWindow()
		{
			AppControl::resizeWindow({});
		}

		void EmbeddedLauncher::killImpl(Shared::ErrorCode cause)
		{
			if (mPID)
			{
				checkProcess();
				if (mPID)
				{
					TerminateProcess(mHandle, -1);
					std::string msg = getName().toStdString() + ": Process killed! (" + Shared::to_string(cause) + ")";
					LOG_ERROR(msg);
					onDisconnected();
				}
			}
		}


		void EmbeddedLauncher::shutdownImpl()
		{
		}

		void EmbeddedLauncher::timerEvent(QTimerEvent* te)
		{
			ApplicationLauncher::timerEvent(te);
			if (mPID)
			{
				checkProcess();
			}
			if (mPID){
				DWORD dwRead;
				CHAR buffer[256];

				bool result = PeekNamedPipe(mChildOutRead, NULL, 0, NULL, &dwRead, NULL);

				if (result)
				{
					QStringList msg;
					while (dwRead > 0)
					{
						DWORD bytesRead;
						result = ReadFile(mChildOutRead, buffer, std::min(sizeof(buffer) - 1, size_t(dwRead)), &bytesRead, NULL);
						assert(result && bytesRead > 0);
						buffer[bytesRead] = '\0';
						msg << QString(buffer);
						dwRead -= bytesRead;
					}

					if (!msg.empty())
					{
						emit outputReceived(msg.join(""));
					}
				}
			}
		}

		bool EmbeddedLauncher::setupImpl()
		{
			return setupImpl(true);

			
		}


		Engine::Serialize::SerializeManager* EmbeddedLauncher::network()
		{
			return &mNetwork;
		}

		void EmbeddedLauncher::sendCommand(const QString& cmd)
		{
			DWORD dwWritten;
			std::string stdCmd = cmd.toStdString();
			stdCmd += '\n';
			bool result = WriteFile(mChildInWrite, stdCmd.c_str(), stdCmd.size(), &dwWritten, NULL);
			assert(result && dwWritten == stdCmd.size());
		}

		void EmbeddedLauncher::onDisconnected()
		{
			ApplicationLauncher::onDisconnected();

			if (mPID)
			{
				mPID = 0;
				CloseHandle(mHandle);
				mHandle = NULL;

				mMemory.mgr()->destroy<Shared::BoostIPCServer>("Server");

			}

			mNetwork.close();

		}

		void EmbeddedLauncher::checkProcess()
		{
			DWORD exitCode = 0;
			if (GetExitCodeProcess(mHandle, &exitCode) == FALSE)
				throw 0;
			if (exitCode != STILL_ACTIVE)
			{
				if (exitCode != 0)
				{
					LOG_ERROR(getName().toStdString() + " returned: " + Shared::to_string(static_cast<Shared::ErrorCode>(exitCode)));
				}
				onDisconnected();
			}
		}


	}
}
