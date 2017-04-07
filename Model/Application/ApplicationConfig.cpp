#include "maditormodellib.h"

#include "ApplicationConfig.h"

#include "Project\ConfigList.h"

#include "ApplicationLauncher.h"

#include "Project\Project.h"

#include "Shared\ApplicationInfo.h"

namespace Maditor {
	namespace Model {
		ApplicationConfig::ApplicationConfig(ConfigList *parent, const QString & name) :
			ProjectElement(name, "Config", parent),
			mParent(parent)
		{

			init();

		}

		ApplicationConfig::ApplicationConfig(QDomElement data, ConfigList *parent) :
			ProjectElement(data, parent),
			mParent(parent)
		{

			init();

		}

		ApplicationConfig::~ApplicationConfig()
		{
		}

		void ApplicationConfig::generateInfo(Shared::ApplicationInfo &appInfo, QWindow * w)
		{
			appInfo.mMediaDir = (path() + "Data/").toStdString().c_str();
			appInfo.mProjectDir = path().toStdString().c_str();
			if (w) {
				appInfo.mWindowHandle = (size_t)(w->winId());
				appInfo.mWindowWidth = w->width();
				appInfo.mWindowHeight = w->height();
			}
		}

		bool ApplicationConfig::hasModuleEnabled(Module * module)
		{
			return true;
		}

		ApplicationLauncher * ApplicationConfig::createInstace()
		{
			return createDocument<ApplicationLauncher>(this);
		}

		void ApplicationConfig::init()
		{
			connect(this, &DocumentStore::documentCreated, this, &ApplicationConfig::onDocumentCreated);
			connect(this, &DocumentStore::documentDestroyed, this, &ApplicationConfig::onDocumentDestroyed);
		}

		void ApplicationConfig::onDocumentCreated(Document * doc)
		{
			emit documentCreated(static_cast<ApplicationLauncher*>(doc));
		}

		void ApplicationConfig::onDocumentDestroyed(Document * doc)
		{
			emit documentDestroyed(static_cast<ApplicationLauncher*>(doc));
		}

		int ApplicationConfig::childCount() const
		{
			return 0;
		}
		TreeItem * ApplicationConfig::child(int i)
		{
			throw 0;
		}
		Project * ApplicationConfig::project()
		{
			return mParent->project();
		}
		QString ApplicationConfig::path() const
		{
			return mParent->path();
		}
	}
}