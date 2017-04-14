#include "maditormodellib.h"

#include "ApplicationConfig.h"

#include "ConfigList.h"

#include "Application\ApplicationLauncher.h"

#include "Project.h"

#include "Shared\ApplicationInfo.h"

#include "ModuleList.h"

#include "Generators\ServerClassGenerator.h"

namespace Maditor {
	namespace Model {
		ApplicationConfig::ApplicationConfig(ConfigList *parent, const QString & name) :
			ProjectElement(name, "Config", parent),
			Document(name),
			Generator(false),
			mParent(parent),
			mLauncher(MADITOR_LAUNCHER),
			mLauncherType(CLIENT_LAUNCHER),
			mServer(nullptr),
			mInstanceCounter(0),
			mModules(this)
		{
			storeData();

			init();
		}

		ApplicationConfig::ApplicationConfig(QDomElement data, ConfigList *parent) :
			ProjectElement(data, parent),
			Document(data.attribute("name")),
			Generator(false),
			mParent(parent),
			mInstanceCounter(0),
			mModules(data.firstChildElement("ModuleSelection"), this)
		{
			restoreData();
			init();
		}

		ApplicationConfig::~ApplicationConfig()
		{
		}

		void ApplicationConfig::generateInfo(Shared::ApplicationInfo &appInfo, QWindow * w)
		{
			appInfo.mMediaDir = (path() + "Data/").toStdString().c_str();
			appInfo.mProjectDir = path().toStdString().c_str();

			if (mLauncherType == CLIENT_LAUNCHER) {
				assert(w);
				appInfo.mWindowHandle = (size_t)(w->winId());
				appInfo.mWindowWidth = w->width();
				appInfo.mWindowHeight = w->height();

				appInfo.mServerClass.clear();
			}
			else {
				appInfo.mServerClass = mServer->fullName().toStdString().c_str();
			}
		}

		bool ApplicationConfig::hasModuleEnabled(Module * module)
		{
			return true;
		}

		ApplicationLauncher * ApplicationConfig::createInstace()
		{
			return mDocuments.createDocument<ApplicationLauncher>(this, name() + ":" + QString::number(++mInstanceCounter));
		}

		ApplicationConfig::Launcher ApplicationConfig::launcher()
		{
			return mLauncher;
		}

		const QString & ApplicationConfig::customExecutableCmd()
		{
			return mCustomExecutableCmd;
		}

		ApplicationConfig::LauncherType ApplicationConfig::launcherType()
		{
			return mLauncherType;
		}

		Generators::ServerClassGenerator * ApplicationConfig::server()
		{
			return mServer;
		}

		void ApplicationConfig::save()
		{
			if (ProjectElement::save() && project()->writeToDisk())
				Document::save();
		}

		void ApplicationConfig::discardChanges()
		{
			ProjectElement::discardChanges();
			Document::discardChanges();
		}

		ModuleSelection * ApplicationConfig::modules()
		{
			return &mModules;
		}

		void ApplicationConfig::setLauncherType(LauncherType type)
		{
			mLauncherType = type;
			setDirtyFlag(true);
			emit launcherTypeChanged(type);
		}

		void ApplicationConfig::setCustomExecutableCmd(const QString & cmd)
		{
			mCustomExecutableCmd = cmd;
			setDirtyFlag(true);
			emit customExecutableCmdChanged(cmd);
		}

		void ApplicationConfig::setServer(Generators::ServerClassGenerator * server)
		{
			mServer = server;
			setDirtyFlag(true);
			emit serverChanged(server);
		}

		void ApplicationConfig::setServerByName(const QString & name)
		{
			setServer(project()->moduleList()->getClass<Generators::ServerClassGenerator>(name));
		}

		void ApplicationConfig::setLauncher(Launcher launcher)
		{
			mLauncher = launcher;
			setDirtyFlag(true);
			emit launcherChanged(launcher);
		}

		void ApplicationConfig::doubleClicked()
		{
			emit mParent->openConfig(this);
		}

		bool ApplicationConfig::storeData()
		{
			element().setAttribute("launcher", mLauncher);
			element().setAttribute("launcherType", mLauncherType);
			element().setAttribute("customExecutableCmd", mCustomExecutableCmd);
			element().setAttribute("server", mServer->fullName());
			mModules.storeData();
			return true;
		}

		void ApplicationConfig::restoreData()
		{
			setLauncher((Launcher)intAttribute("launcher", MADITOR_LAUNCHER));			
			setLauncherType((LauncherType)intAttribute("launcherType", CLIENT_LAUNCHER));			
			setCustomExecutableCmd(stringAttribute("customExecutableCmd"));
			setServerByName(stringAttribute("server"));
			mModules.restoreData();
			setDirtyFlag(false);
		}

		void ApplicationConfig::init()
		{
			connect(&mDocuments, &DocumentStore::documentCreated, this, &ApplicationConfig::onDocumentCreated);
			connect(&mDocuments, &DocumentStore::documentDestroyed, this, &ApplicationConfig::onDocumentDestroyed);
		}

		void ApplicationConfig::onDocumentCreated(Document * doc)
		{
			emit documentCreated(static_cast<ApplicationLauncher*>(doc));
		}

		void ApplicationConfig::onDocumentDestroyed(Document * doc)
		{
			emit documentDestroyed(static_cast<ApplicationLauncher*>(doc));
		}

		QStringList ApplicationConfig::filePaths()
		{
			return{ path() + "src/main.cpp" };
		}

		void ApplicationConfig::write(QTextStream & stream, int index)
		{
			QString content = templateFile("main.cpp");

			stream << content;
		}

	}
}