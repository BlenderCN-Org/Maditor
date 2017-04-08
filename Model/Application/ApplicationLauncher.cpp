#include "maditormodellib.h"

#include "ApplicationLauncher.h"

#include "OgreWindow.h"

#include "Shared\SharedMemory.h"

#include "Network\networkmanager.h"

#include "InputWrapper.h"

#include "ApplicationConfig.h"

#include "Project\Project.h"

namespace Maditor {
	namespace Model {
		ApplicationLauncher::ApplicationLauncher(ApplicationConfig *config, const QString &uniqueName) :
			Document(uniqueName),
			mInput(new InputWrapper(sharedMemory().mInput)),
			mWindow(config->launcher() == ApplicationConfig::MADITOR_LAUNCHER ? new OgreWindow(mInput.get()) : nullptr),
			mLoader(config),
			mPID(0),
			mLog(config->project()->logs(), std::list<std::string>{ "Ogre.log" }),
			mWaitingForLoader(false),			
			mAboutToBeDestroyed(false),
			mRunning(false),
			mSetup(false),
			mConfig(config)
		{
			if (mWindow)
				connect(mWindow, &OgreWindow::resized, this, &ApplicationLauncher::resizeWindow);

			startTimer(10);

			mPingTimer.setSingleShot(true);

			connect(&mPingTimer, &QTimer::timeout, this, &ApplicationLauncher::timeout);

			postConstruct();
		}

		ApplicationLauncher::~ApplicationLauncher()
		{
			kill(CLEANUP);
		}

		void ApplicationLauncher::destroy()
		{
			if (!mPID) {
				Document::destroy();
			}
			else {
				mAboutToBeDestroyed = true;
				connect(&mPingTimer, &QTimer::timeout, this, &ApplicationLauncher::destroy);
				shutdown();
				mPingTimer.start(3000);
			}
		}

		void ApplicationLauncher::setup()
		{
			setupImpl(true);
		}

		void ApplicationLauncher::setupImpl(bool debug)
		{
			
			Shared::ApplicationInfo &appInfo = sharedMemory().mAppInfo;
			appInfo.mDebugged = debug;

			std::string cmd;

			if (mConfig->launcher() == ApplicationConfig::MADITOR_LAUNCHER) {
				emit applicationSettingup();

				mConfig->generateInfo(appInfo, mWindow);

				cmd = std::string("Maditor_Launcher.exe ") + std::to_string(appId());
			}
			else {
				cmd = mConfig->customExecutableCmd().toStdString();
			}

			STARTUPINFO si;
			PROCESS_INFORMATION pi;

			// set the size of the structures
			ZeroMemory(&si, sizeof(si));
			si.cb = sizeof(si);
			ZeroMemory(&pi, sizeof(pi));			

			// start the program up
			CreateProcess(NULL,   // the path
				const_cast<char*>(cmd.c_str()),
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
			mUtil->stats()->setProcess(mHandle);

			if (mConfig->launcher() == ApplicationConfig::MADITOR_LAUNCHER) {				
				Engine::Network::NetworkManager *net = network();
				size_t id = appInfo.waitForAppId();
				if (!id) {
					kill(NO_APPLICATION_NOTIFICATION);
					return;
				}
				setStaticSlaveId(id);
				if (!net->connect("127.0.0.1", 1000, 1000)) {
					kill(FAILED_CONNECTION);
					return;
				}

				mLoader->setup();

				mWaitingForLoader = true;

				if (!debug)
					pingImpl();
			}
			else {
				onApplicationSetup();
			}
		}
		void ApplicationLauncher::setupNoDebug()
		{
			setupImpl(false);
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
			mRunning = true;
			emit applicationStarted();
		}
		void ApplicationLauncher::stopImpl()
		{
			mRunning = false;
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
			return mUtil.ptr();
		}

		DWORD ApplicationLauncher::pid()
		{
			return mPID;
		}

		bool ApplicationLauncher::isRunning()
		{
			return mRunning;
		}

		bool ApplicationLauncher::isLaunched()
		{
			return mPID != 0;
		}

		bool ApplicationLauncher::isSetup()
		{
			return mSetup;
		}

		bool ApplicationLauncher::needsWindow()
		{
			return mConfig->launcher() == Model::ApplicationConfig::MADITOR_LAUNCHER && mConfig->launcherType() == Model::ApplicationConfig::CLIENT_LAUNCHER;
		}

		void ApplicationLauncher::timerEvent(QTimerEvent * te)
		{
			if (mPID) {
				DWORD exitCode = 0;
				if (GetExitCodeProcess(mHandle, &exitCode) == FALSE)
					throw 0;
				if (exitCode != STILL_ACTIVE) {
					if (exitCode != 0) {
						LOG_ERROR(std::string("Application returned: ") + std::to_string(static_cast<long>(exitCode)));
					}
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
			kill(USER_REQUEST);
		}

		void ApplicationLauncher::kill(KillCause cause)
		{
			if (mPID) {
				TerminateProcess(mHandle, -1);
				std::string msg = "Process killed! (";
				switch (cause) {
				case USER_REQUEST:
					msg += "User request";
					break;
				case TIMEOUT:
					msg += "No Response from App";
					break;
				case FAILED_CONNECTION:
					msg += "Unable to connect to process";
					break;
				case NO_APPLICATION_NOTIFICATION:
					msg += "Application data not set up correctly";
					break;
				default:
					msg += "unknown cause";
				}
				msg += ")";
				LOG_ERROR(msg);
				cleanup();
			}
		}

		void ApplicationLauncher::shutdown()
		{
			if (mPID) {
				AppControl::stop({});
				mPingTimer.stop();
				AppControl::shutdown({});
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
				mUtil->reset();		

				mPingTimer.stop();

				mSetup = false;
				emit applicationShutdown();
			}			
			if (mAboutToBeDestroyed)
				destroy();
		}

		void ApplicationLauncher::shutdownImpl()
		{
			 
		}


		void ApplicationLauncher::onApplicationSetup()
		{
			mSetup = true;
			emit applicationSetup();
		}

		void ApplicationLauncher::pingImpl()
		{
			mPingTimer.start(3000);
			ping({});
		}

		void ApplicationLauncher::timeout()
		{
			kill(TIMEOUT);
		}

		void ApplicationLauncher::resizeWindow() {
			AppControl::resizeWindow({});
		}

		size_t ApplicationLauncher::getSize() const {
			return sizeof(ApplicationLauncher);
		}

	}
}