#include "maditormodellib.h"

#include "ApplicationLauncher.h"

#include "OgreWindow.h"

#include "Shared\SharedMemory.h"

#include "Shared\IPCManager\boostIPCmanager.h"
#include "Shared\IPCManager\boostIPCServer.h"

#include "InputWrapper.h"

#include "Project\ApplicationConfig.h"

#include "Project\Project.h"

#include <iostream>

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
			mConfig(config),
			mName(uniqueName),
			mChildInRead(NULL),
			mChildInWrite(NULL),
			mChildOutRead(NULL),
			mChildOutWrite(NULL)
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
			kill(Shared::KILL_CLEANUP);
		}

		void ApplicationLauncher::destroy()
		{
			if (!mPID) {
				Document::destroy();
			}
			else {
				if (isLauncher()) {
					mAboutToBeDestroyed = true;
					connect(&mPingTimer, &QTimer::timeout, this, &ApplicationLauncher::destroy);
					shutdown();
					mPingTimer.start(3000);
				}
				else {
					kill(Shared::KILL_USER_REQUEST);
					Document::destroy();
				}
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
			appInfo.mAppName = mName.toStdString().c_str();

			std::string cmd;

			emit applicationSettingup();
			if (mConfig->launcher() == ApplicationConfig::MADITOR_LAUNCHER) {

				mConfig->generateInfo(appInfo, mWindow);

				cmd = std::string("Maditor_Launcher.exe ") + std::to_string(appId());
			}
			else {
				cmd = mConfig->customExecutableCmd().toStdString();
			}

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
			bool result = CreateProcess(NULL,   // the path
				const_cast<char*>(cmd.c_str()),
				NULL,           // Process handle not inheritable
				NULL,           // Thread handle not inheritable
				TRUE,          // Set handle inheritance to FALSE
				0,              // No creation flags
				NULL,           // Use parent's environment block
				NULL,           // Use parent's starting directory 
				&si,            // Pointer to STARTUPINFO structure
				&pi           // Pointer to PROCESS_INFORMATION structure
			);

			if (!result)
				return;

			mPID = pi.dwProcessId;
			mHandle = pi.hProcess;
			CloseHandle(pi.hThread);

			emit processStarted(mPID, appInfo);
			mUtil->stats()->setProcess(mHandle);

			if (mConfig->launcher() == ApplicationConfig::MADITOR_LAUNCHER) {				
				Shared::BoostIPCManager *net = network();
				size_t id = appInfo.waitForAppId();
				if (!id) {
					kill(Shared::KILL_NO_APPLICATION_NOTIFICATION);
					return;
				}
				setStaticSlaveId(id);
				if (!net->connect(1000)) {
					kill(Shared::KILL_FAILED_CONNECTION);
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

		bool ApplicationLauncher::isClient()
		{
			return isLauncher() && mConfig->launcherType() == Model::ApplicationConfig::CLIENT_LAUNCHER;
		}

		bool ApplicationLauncher::isLauncher()
		{
			return mConfig->launcher() == Model::ApplicationConfig::MADITOR_LAUNCHER;
		}

		void ApplicationLauncher::sendCommand(const QString & cmd)
		{
			DWORD dwWritten;
			std::string stdCmd = cmd.toStdString();
			stdCmd += '\n';
			bool result = WriteFile(mChildInWrite, stdCmd.c_str(), stdCmd.size(), &dwWritten, NULL);
			assert(result && dwWritten == stdCmd.size());
		}

		void ApplicationLauncher::timerEvent(QTimerEvent * te)
		{
			if (mPID) {
				network()->receiveMessages();
				if (mWaitingForLoader && mLoader->done()) {
					mLoader->setup2();
					mWaitingForLoader = false;
				}		

				DWORD dwRead;
				CHAR buffer[256];

				bool result = PeekNamedPipe(mChildOutRead, NULL, 0, NULL, &dwRead, NULL);
				assert(result);

				if (dwRead > 0){
					result = ReadFile(mChildOutRead, buffer, std::min(sizeof(buffer)-1, size_t(dwRead)), &dwRead, NULL);
					assert(result && dwRead > 0);
					buffer[dwRead] = '\0';
					emit outputReceived(buffer);
				}

				checkProcess();
			}
		}

		void ApplicationLauncher::kill()
		{
			kill(Shared::KILL_USER_REQUEST);
		}

		void ApplicationLauncher::kill(Shared::ErrorCode cause)
		{
			if (mPID) {
				checkProcess();
				if (mPID) {
					TerminateProcess(mHandle, -1);
					std::string msg = mName.toStdString() + ": Process killed! (" + Shared::to_string(cause) + ")";
					LOG_ERROR(msg);
					cleanup();
				}
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
			if (mPID) {
				mPID = 0;
				CloseHandle(mHandle);
				mHandle = NULL;

				//receive pending messages
				while (network()->getSlaveStream() && network()->getSlaveStream()->isMessageAvailable())
					network()->receiveMessages();

				mLoader->reset();
				mWaitingForLoader = false;
				mUtil->reset();		

				mPingTimer.stop();

				mem()->mgr()->destroy<Shared::BoostIPCServer>("Server");

				mSetup = false;
				emit applicationShutdown();
			}
			network()->close();
			if (mChildInRead) {
				CloseHandle(mChildInRead);
				mChildInRead = NULL;
			}
			if (mChildInWrite) {
				CloseHandle(mChildInWrite);
				mChildInWrite = NULL;
			}
			if (mChildOutRead) {
				CloseHandle(mChildOutRead);
				mChildOutRead = NULL;
			}
			if (mChildOutWrite) {
				CloseHandle(mChildOutWrite);
				mChildOutWrite = NULL;
			}
			if (mAboutToBeDestroyed)
				destroy();
		}

		void ApplicationLauncher::checkProcess()
		{
			DWORD exitCode = 0;
			if (GetExitCodeProcess(mHandle, &exitCode) == FALSE)
				throw 0;
			if (exitCode != STILL_ACTIVE) {
				if (exitCode != 0) {
					LOG_ERROR(mName.toStdString() + " returned: " + Shared::to_string(static_cast<Shared::ErrorCode>(exitCode)));
				}
				cleanup();
			}
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
			kill(Shared::KILL_PING_TIMEOUT);
		}

		void ApplicationLauncher::resizeWindow() {
			AppControl::resizeWindow({});
		}

		size_t ApplicationLauncher::getSize() const {
			return sizeof(ApplicationLauncher);
		}

	}
}