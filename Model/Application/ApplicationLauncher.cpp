#include "maditormodellib.h"

#include "ApplicationLauncher.h"

#include "OgreWindow.h"

#include "Shared\SharedMemory.h"

#include "Network\networkmanager.h"

#include "InputWrapper.h"


namespace Maditor {
	namespace Model {
		ApplicationLauncher::ApplicationLauncher(const QString &path, const ModuleList &modules) :
			AppControl(true),
			mInput(new InputWrapper(sharedMemory().mInput)),
			mWindow(new OgreWindow(mInput)),
			mPath(path),
			mLoader(this, path + "debug/bin/", modules),
			mPID(0)
		{
			network()->addTopLevelItem(&mUtil);

			startTimer(10);
		}

		ApplicationLauncher::~ApplicationLauncher()
		{
			shutdown();
			delete mWindow;
			delete mInput;
		}

		void ApplicationLauncher::init()
		{
			initImpl(true);
		}

		void ApplicationLauncher::initImpl(bool debug)
		{
			emit applicationInitializing();

			Shared::ApplicationInfo &appInfo = sharedMemory().mAppInfo;

			appInfo.mMediaDir = (mPath + "Data/").toStdString().c_str();
			appInfo.mProjectDir = mPath.toStdString().c_str();
			appInfo.mWindowHandle = (size_t)(mWindow->winId());
			appInfo.mWindowWidth = mWindow->width();
			appInfo.mWindowHeight = mWindow->height();
			appInfo.mDebugged = debug;

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
				f(mPID, appInfo);
			}

			Engine::Network::NetworkManager *net = network();
			
			if (!net->connect("127.0.0.1", 1000, 2000)) {
				shutdown();
				return;
			}

			mLoader->setup();

			QTime myTimer;
			myTimer.start();
			
			QEventLoop loop;

			while (!mLoader->done()) {
				net->receiveMessages();
				loop.processEvents();
				if (myTimer.elapsed() > 10000 || !mPID) {
					shutdown();
					return;
				}
			}
			mLoader->setup2();			
		}
		void ApplicationLauncher::initNoDebug()
		{
			initImpl(false);
		}
		void ApplicationLauncher::finalize()
		{
			shutdown();
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
			emit applicationStarted();
		}
		void ApplicationLauncher::stopImpl()
		{
			emit applicationStopped();
		}
		void ApplicationLauncher::pauseImpl()
		{
		}

		OgreWindow * ApplicationLauncher::window()
		{
			return mWindow;
		}

		ModuleLoader * ApplicationLauncher::moduleLoader()
		{
			return mLoader.ptr();
		}

		UtilModel * ApplicationLauncher::util()
		{
			return &mUtil;
		}

		void ApplicationLauncher::addProcessListener(std::function<void(DWORD, const Shared::ApplicationInfo &)> f)
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
				AppControl::stop();
				AppControl::shutdown();
			}
		}

		void ApplicationLauncher::cleanup()
		{
			if (mPID) {
				mPID = 0;
				CloseHandle(mHandle);

				mLoader->reset();
				mUtil.reset();

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