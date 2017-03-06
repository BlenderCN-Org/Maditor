#include "maditormodellib.h"

#include "ApplicationLauncher.h"

#include "OgreWindow.h"

#include "Shared\SharedMemory.h"

#include "Network\networkmanager.h"

#include "InputWrapper.h"


namespace Maditor {
	namespace Model {
		ApplicationLauncher::ApplicationLauncher(const QString &path, const ModuleList &modules, LogsModel *logs) :
			AppControl(true),
			mInput(new InputWrapper(sharedMemory().mInput)),
			mWindow(new OgreWindow(mInput.get())),
			mPath(path),
			mLoader(this, path + "debug/bin/", modules),
			mPID(0),
			mLog(this, logs, std::list<std::string>{ "Ogre.log" }),
			mWaitingForLoader(false),
			mUtil(false)
		{
			network()->addTopLevelItem(&mUtil);

			connect(mWindow, &OgreWindow::resized, this, &ApplicationLauncher::resizeWindow);

			startTimer(10);

			mPingTimer.setSingleShot(true);

			connect(&mPingTimer, &QTimer::timeout, this, &ApplicationLauncher::kill);
		}

		ApplicationLauncher::~ApplicationLauncher()
		{
			kill();
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

			emit processStarted(mPID, appInfo);
			mUtil.stats()->setProcess(mHandle);

			Engine::Network::NetworkManager *net = network();
			
			if (!net->connect("127.0.0.1", 1000, 2000)) {
				kill();
				return;
			}

			mLoader->setup(false);

			mWaitingForLoader = true;	

			if (!debug)
				pingImpl();
		}
		void ApplicationLauncher::initNoDebug()
		{
			initImpl(false);
		}
		void ApplicationLauncher::start()
		{
			AppControl::start({});
		}
		void ApplicationLauncher::pause()
		{
			AppControl::pause({});
		}
		void ApplicationLauncher::stop()
		{
			AppControl::stop({});
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
				if (mLoader->done() && mWaitingForLoader) {
					mLoader->setup2();
					mWaitingForLoader = false;
				}				
			}
		}

		void ApplicationLauncher::kill()
		{
			if (mPID) {
				TerminateProcess(mHandle, -1);
				cleanup();
			}
		}

		void ApplicationLauncher::shutdown()
		{
			if (mPID) {
				AppControl::stop({});
				AppControl::shutdown({});
				/*if (!mPingTimer.isActive())
					pingImpl();*/
			}
		}

		void ApplicationLauncher::cleanup()
		{
			network()->close();
			if (mPID) {
				mPID = 0;
				CloseHandle(mHandle);

				mLoader->reset();
				mWaitingForLoader = false;
				mUtil.reset();

				mPingTimer.stop();

				emit applicationShutdown();
			}			
		}

		void ApplicationLauncher::shutdownImpl()
		{
			 
		}


		void ApplicationLauncher::onApplicationInitialized()
		{
			emit applicationInitialized();
		}

		void ApplicationLauncher::pingImpl()
		{
			mPingTimer.start(10000);
			ping({});
		}

		void ApplicationLauncher::resizeWindow() {
			AppControl::resizeWindow({});
		}

	}
}