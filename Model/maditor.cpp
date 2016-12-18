#include "maditormodellib.h"

#include "maditor.h"

#include "DialogManager.h"

namespace Maditor {
	namespace Model {

		DialogManager *DialogManager::sSingleton = 0;

		Maditor::Maditor() :
			mSettings("MadMan Studios", "Maditor")			
		{

			mSettings.beginGroup("Editor");
			mRecentProjects = mSettings.value("recentProjects").toStringList();
			mReloadProject = mSettings.value("reloadProject").toBool();
			mSettings.endGroup();

			if (mReloadProject && !mRecentProjects.isEmpty()) {
				loadProject(mRecentProjects.front());
			}
		}



		Maditor::~Maditor()
		{
			mSettings.beginGroup("Editor");
			mSettings.setValue("recentProjects", mRecentProjects);
			mSettings.setValue("reloadProject", mReloadProject);
			mSettings.endGroup();

		}

		void Maditor::newProject(const QString &path, const QString &name)
		{

			openProject(std::make_unique<Project>(path, name));

		}

		void Maditor::loadProject(const QString & path)
		{

			openProject(std::unique_ptr<Project>(Project::load(path)));

		}

		void Maditor::openProject(std::unique_ptr<Project> &&project)
		{
			if (!project->isValid()) return;

			mProject = std::forward<std::unique_ptr<Project>>(project);

			QString path = mProject->path();
			mRecentProjects.removeAll(path);
			mRecentProjects.push_front(path);

			emit recentProjectsChanged(mRecentProjects);


			emit projectOpened(mProject.get());

			//mEditorManager->setCurrentRoot(mProject->moduleList()->path());

			//mApplicationWrapper.go();
		}

		void Maditor::clearRecentProjects()
		{
			mRecentProjects.clear();
			emit recentProjectsChanged(mRecentProjects);
		}


		QSettings & Maditor::settings()
		{
			return mSettings;
		}

		const QStringList & Maditor::recentProjects()
		{
			return mRecentProjects;
		}

		bool Maditor::reloadProjectProperty()
		{
			return mReloadProject;
		}

		void Maditor::setReloadProjectProperty(bool b)
		{
			mReloadProject = b;
		}

		Project * Maditor::project()
		{
			return mProject.get();
		}

	}
}

