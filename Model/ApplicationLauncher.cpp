#include "maditormodellib.h"

#include "ApplicationLauncher.h"

#include "OgreWindow.h"

#include "Shared\SharedMemory.h"

#include "Network\networkmanager.h"


namespace Maditor {
	namespace Model {
		ApplicationLauncher::ApplicationLauncher(const QString &path, const ModuleList &modules) :
			AppControl(true),
			mWindow(new OgreWindow),
			mPath(path),
			mLoader(this, path + "debug/bin/", modules),
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

			for (const auto& f : mProcessListener) {
				f(mPID);
			}

			Engine::Network::NetworkManager *net = network();
			
			if (!net->connect("127.0.0.1", 1000, 2000)) {
				shutdown();
				return;
			}

			mLoader->setup();
			while (!mLoader->done())
				net->receiveMessages();
			mLoader->setup2();
			//shutdown();
			
		}
		void ApplicationLauncher::start()
		{
			AppControl::start();
		}
		void ApplicationLauncher::pause()
		{
			AppControl::pause();
		}
		void ApplicationLauncher::stop()
		{
			AppControl::stop();
		}

		void ApplicationLauncher::startImpl()
		{
		}
		void ApplicationLauncher::stopImpl()
		{
		}
		void ApplicationLauncher::pauseImpl()
		{
		}

		OgreWindow * ApplicationLauncher::window()
		{
			return mWindow;
		}

		void ApplicationLauncher::addProcessListener(std::function<void(DWORD)> f)
		{
			mProcessListener.push_back(f);
		}

		DWORD ApplicationLauncher::pid()
		{
			return mPID;
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
				network()->receiveMessages();
			}
		}

		void ApplicationLauncher::shutdown()
		{
			if (mPID) {
				AppControl::shutdown();
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
			network()->close();
		}

		void ApplicationLauncher::shutdownImpl()
		{
			 
		}


		void ApplicationLauncher::onApplicationInitialized()
		{
			emit applicationInitialized();
		}

	}
}