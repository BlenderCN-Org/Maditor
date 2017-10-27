#include "maditormodellib.h"

#include "ApplicationLauncher.h"

#include "OgreWindow.h"

#include "Shared\SharedMemory.h"

#include "Shared\IPCManager\boostIPCmanager.h"
#include "Shared\IPCManager\boostIPCServer.h"

#include "InputWrapper.h"

#include "Project\ApplicationConfig.h"

#include "Project\Project.h"

#include "Serialize/toplevelids.h"


namespace Maditor {
	namespace Model {
		ApplicationLauncher::ApplicationLauncher(ApplicationConfig *config, const QString &uniqueName) :
			Document(uniqueName),
			AppControl(),
			mNetwork(&mMemory, uniqueName.toStdString()),
			mInput(new InputWrapper(mMemory.data().mInput)),
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
			mChildOutWrite(NULL),
			mPong(false),
			mOnConnectionResult(this)
		{
			mNetwork.connectConnectionSlot(mOnConnectionResult);

			mNetwork.addTopLevelItem(this);

			if (mWindow)
				connect(mWindow, &OgreWindow::resized, this, &ApplicationLauncher::resizeWindow);

			startTimer(10);

			mPingTimer.setSingleShot(true);

			connect(&mPingTimer, &QTimer::timeout, this, &ApplicationLauncher::timeout);

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

		ApplicationLauncher::~ApplicationLauncher()
		{
			kill(Shared::KILL_CLEANUP);

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
			
			Shared::ApplicationInfo &appInfo = mMemory.data().mAppInfo;
			appInfo.mDebugged = debug;
			appInfo.mAppName = mName.toStdString().c_str();

			std::string cmd;

			emit applicationSettingup();
			if (isLauncher()) {

				mConfig->generateInfo(appInfo, mWindow);

				cmd = isClient() ? "Client_Launcher.exe " : "Server_Launcher.exe ";

				cmd += std::to_string(mMemory.id());
			}
			else {
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
			bool result = CreateProcess(NULL,   // the path
				const_cast<char*>(cmd.c_str()),
				NULL,           // Process handle not inheritable
				NULL,           // Thread handle not inheritable
				TRUE,          // Set handle inheritance to TRUE
				0,              // No creation flags
				NULL,           // Use parent's environment block
				appInfo.mProjectDir.empty() ? NULL : runtimeDir().c_str(),
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

			if (isLauncher()) {				
				Shared::BoostIPCManager *net = &mNetwork;
				setStaticSlaveId(Engine::Serialize::MADITOR);
				net->connect_async(10000);
			}
			else {
				onApplicationSetup();
			}
		}

		void ApplicationLauncher::onConnectionResult(bool b)
		{
			if (!b) {
				kill(Shared::KILL_FAILED_CONNECTION);
			}
			else {
				mLoader->setup();
				mWaitingForLoader = true;
			}
		}

		std::string ApplicationLauncher::runtimeDir() {
			return (mConfig->project()->path() + "debug/runtime/" + mName + "/").toStdString();
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

		void ApplicationLauncher::sendLua(const QString & cmd)
		{
			execLua(cmd.toStdString(), {});
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

		Inspector * ApplicationLauncher::inspector()
		{
			return mInspector.ptr();
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
				mNetwork.sendAndReceiveMessages();
				if (mWaitingForLoader && mLoader->done()) {
					mLoader->setup2();
					mWaitingForLoader = false;
				}		

				checkProcess();
			}

			if (mPID) {

				DWORD dwRead;
				CHAR buffer[256];

				bool result = PeekNamedPipe(mChildOutRead, NULL, 0, NULL, &dwRead, NULL);

				if (result) {
					QStringList msg;
					while (dwRead > 0) {
						DWORD bytesRead;
						result = ReadFile(mChildOutRead, buffer, std::min(sizeof(buffer) - 1, size_t(dwRead)), &bytesRead, NULL);
						assert(result && bytesRead > 0);
						buffer[bytesRead] = '\0';
						msg << QString(buffer);
						dwRead -= bytesRead;
					}

					if (!msg.empty()) {
						emit outputReceived(msg.join(""));
					}					
				}
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
				while (!mNetwork.isMaster() && mNetwork.getSlaveStream()->isMessageAvailable())
					mNetwork.receiveMessages();

				mInspector->reset();
				mLoader->reset();
				mWaitingForLoader = false;
				mUtil->reset();		

				mPingTimer.stop();

				mMemory.mgr()->destroy<Shared::BoostIPCServer>("Server");

				mSetup = false;
				emit applicationShutdown();
			}
			static_cast<Shared::BoostIPCManager&>(mNetwork).close();

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
			if (isLauncher() && !mMemory.data().mAppInfo.mDebugged) {
				ping({});
				mPingTimer.start(3000);
			}
			mInspector->start();
			mSetup = true;
			emit applicationSetup();
		}

		void ApplicationLauncher::pingImpl()
		{
			mPong = true;
		}

		void ApplicationLauncher::timeout()
		{
			if (mPong) {
				mPong = false;
				ping({});
				mPingTimer.start(3000);
			}
			else {
				kill(Shared::KILL_PING_TIMEOUT);
			}
		}

		void ApplicationLauncher::resizeWindow() {
			AppControl::resizeWindow({});
		}

		size_t ApplicationLauncher::getSize() const {
			return sizeof(ApplicationLauncher);
		}

	}
}