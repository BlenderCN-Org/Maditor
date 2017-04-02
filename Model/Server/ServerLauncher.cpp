#include "maditormodellib.h"

#include "ServerLauncher.h"

#include "Shared\SharedMemory.h"

#include "Network\networkmanager.h"

#include "Shared\ServerInfo.h"

#include "Project\Generators\ServerClassGenerator.h"


namespace Maditor {
	namespace Model {
		ServerLauncher::ServerLauncher(Generators::ServerClassGenerator *serverClass, const QString &path, const ModuleList &modules) :			
			mPath(path),
			mLoader(path + "debug/bin/", modules),
			mPID(0),
			mLogReader(&mLogs, std::list<std::string>{ "TW-Main-Server-Log" }),
			mWaitingForLoader(false),
			ServerControl(true),
			mServerClass(serverClass),
			mUtil(true)
		{
			postConstruct();

			startTimer(10);

			mPingTimer.setSingleShot(true);

			connect(&mPingTimer, &QTimer::timeout, this, &ServerLauncher::kill);
		}

		ServerLauncher::~ServerLauncher()
		{
			kill();
		}

		void ServerLauncher::start()
		{
			startImpl(true);
		}

		void ServerLauncher::startImpl(bool debug)
		{

			emit serverStarting();

			if (mPID)
				return;

			mLogReader->clear();

			Shared::ServerInfo &serverInfo = sharedMemory();

			serverInfo.mServerClass = mServerClass->fullName().toStdString().c_str();
			serverInfo.mMediaDir = (mPath + "Data/Media/Scripts/").toStdString().c_str();
			serverInfo.mProjectDir = mPath.toStdString().c_str();
			serverInfo.mDebugged = debug;

			STARTUPINFO si;
			PROCESS_INFORMATION pi;

			// set the size of the structures
			ZeroMemory(&si, sizeof(si));
			si.cb = sizeof(si);
			ZeroMemory(&pi, sizeof(pi));

			// start the program up
			CreateProcess(NULL,   // the path
				"Maditor_Server_Launcher.exe",
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

			emit processStarted(mPID, serverInfo);
			mUtil->stats()->setProcess(mHandle);

			Engine::Network::NetworkManager *net = network();
			
			if (!net->connect("127.0.0.1", 1001, 2000)) {
				shutdown();
				return;
			}

			mLoader->setup(true);

			mWaitingForLoader = true;	

			emit serverStarted();

			if (!debug)
				pingImpl();
		}
		void ServerLauncher::startNoDebug()
		{
			startImpl(false);
		}
		

		ModuleLoader * ServerLauncher::moduleLoader()
		{
			return mLoader.ptr();
		}

		UtilModel * ServerLauncher::util()
		{
			return mUtil.ptr();
		}

		LogsModel * ServerLauncher::logs()
		{
			return &mLogs;
		}


		DWORD ServerLauncher::pid()
		{
			return mPID;
		}

		QString ServerLauncher::name()
		{
			return mServerClass->fullName();
		}

		void ServerLauncher::timerEvent(QTimerEvent * te)
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

		void ServerLauncher::kill()
		{
			if (mPID) {
				TerminateProcess(mHandle, -1);
				cleanup();
			}
		}

		void ServerLauncher::shutdown()
		{
			if (mPID) {
				ServerControl::shutdown({});
				/*if (!mPingTimer.isActive())
					pingImpl();*/
			}
		}

		void ServerLauncher::cleanup()
		{
			network()->close();
			if (mPID) {
				mPID = 0;
				CloseHandle(mHandle);

				mLoader->reset();
				mWaitingForLoader = false;
				mUtil->reset();

				mPingTimer.stop();

				emit serverShutdown();
			}			
		}

		void ServerLauncher::shutdownImpl()
		{
			 
		}

		void ServerLauncher::pingImpl()
		{
			mPingTimer.start(10000);
			ping({});
		}

		size_t ServerLauncher::getSize() const {
			return sizeof(ServerLauncher);
		}

	}
}