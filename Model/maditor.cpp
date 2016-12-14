#include "maditormodellib.h"

#include "maditor.h"

#include "DialogManager.h"

namespace Maditor {
	namespace Model {

		DialogManager *DialogManager::sSingleton = 0;

		Maditor::Maditor()
			
		{
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

			/*QString path = mProject->path();
			mRecentProjects.removeAll(path);
			mRecentProjects.push_front(path);*/

			//emit recentProjectsChanged(mRecentProjects);


			emit projectOpened(mProject.get());

			//mEditorManager->setCurrentRoot(mProject->moduleList()->path());

			//mApplicationWrapper.go();
		}

	}
}

