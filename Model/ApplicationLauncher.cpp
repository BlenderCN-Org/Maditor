#include "maditormodellib.h"

#include "ApplicationLauncher.h"

#include "OgreWindow.h"

#include "Shared\SharedMemory.h"

#include "Network\networkmanager.h"

#include "ModuleLoader.h"

namespace Maditor {
	namespace Model {
		ApplicationLauncher::ApplicationLauncher(const QString &path, const ModuleList &modules) :
			AppControl(true),
			mWindow(new OgreWindow),
			mPath(path),
			mLoader(new ModuleLoader(path + "debug/bin/", modules)),
			mPID(0)
		{
			startTimer(500);
		}

		ApplicationLauncher::~ApplicationLauncher()
		{
			shutdown();
		}

		void ApplicationLauncher::init()
		{
			emit applicationInitializing();

			Shared::ApplicationInfo &appInfo = sharedMemory().mAppInfo;

			appInfo.mMediaDir = (mPath + "Data/").toStdString().c_str();
			appInfo.mProjectDir = mPath.toStdString().c_str();
			appInfo.mWindowHandle = (size_t)(mWindow->winId());
			appInfo.mWindowWidth = mWindow->width();
			appInfo.mWindowHeight = mWindow->height();

			STARTUPINFO si;
			PROCESS_INFORMATION pi;

			// set the size of the structures
			ZeroMemory(&si, sizeof(si));
			si.cb = sizeof(si);
			ZeroMemory(&pi, sizeof(pi));

			// start the program up
			CreateProcess(NULL,   // the path
				"Maditor_Launcher.exe",
				NULL,           // Process handle not inheritable
				NULL,           // Thread handle not inheritable
				FALSE,          // Set handle inheritance to FALSE
				0,              // No creation flags
				NULL,           // Use parent's environment block
				NULL,           // Use parent's starting directory 
				&si,            // Pointer to STARTUPINFO structure
				&pi           // Pointer to PROCESS_INFORMATION structure
			);

			mPID = pi.dwProcessId;
			mHandle = pi.hProcess;
			CloseHandle(pi.hThread);

			/*for (const auto& f : mProcessListener) {
				f(mPID);
			}*/

			Engine::Network::NetworkManager *net = network();

			net->addTopLevelItem(this);
			
			if (!net->connect("127.0.0.1", 1000, 2000)) {
				shutdown();
			}


			//shutdown();
			
		}
		OgreWindow * ApplicationLauncher::window()
		{
			return mWindow;
		}

		void ApplicationLauncher::timerEvent(QTimerEvent * te)
		{
			if (mPID) {
				DWORD exitCode = 0;
				if (GetExitCodeProcess(mHandle, &exitCode) == FALSE)
					throw 0;
				if (exitCode != STILL_ACTIVE) {
					cleanup();
					return;
				}
			}
		}

		void ApplicationLauncher::shutdown()
		{
			if (mPID) {
				AppControl::shutdown();
				network()->close();
			}
			cleanup();
		}

		void ApplicationLauncher::cleanup()
		{
			if (mPID) {
				mPID = 0;
				CloseHandle(mHandle);
				emit applicationShutdown();
			}
		}

		void ApplicationLauncher::shutdownImpl()
		{
			 
		}

	}
}