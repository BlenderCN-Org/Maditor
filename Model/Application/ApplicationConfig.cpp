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

		ApplicationLauncher * ApplicationConfig::createInstance()
		{
			ApplicationLauncher *instance = new ApplicationLauncher(this);
			mInstances.emplace_back(instance);
			connect(instance, &ApplicationLauncher::destroyApplication, this, &ApplicationConfig::destroyInstance);
			emit instanceCreated(instance);
			return instance;
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

		void ApplicationConfig::init()
		{
		}

		void ApplicationConfig::destroyInstance(ApplicationLauncher * instance)
		{
			auto it = std::find_if(mInstances.begin(), mInstances.end(), [=](const std::unique_ptr<ApplicationLauncher> &app) {return app.get() == instance; });
			assert(it != mInstances.end());
			emit instanceDestroyed(instance);
			mInstances.erase(it);
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