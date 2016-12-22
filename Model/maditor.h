#pragma once

#include "Project/Project.h"

namespace Maditor {
	namespace Model {

		class MADITOR_MODEL_EXPORT Maditor : public QObject {
			Q_OBJECT

		public:
			Maditor();
			virtual ~Maditor();

			void newProject(const QString &path, const QString &name);
			void loadProject(const QString &path);

			void clearRecentProjects();


			QSettings &settings();

			const QStringList &recentProjects();
			bool reloadProjectProperty();
			void setReloadProjectProperty(bool b);
			
			Project *project();

			Addons::AddonCollector *addons();

		signals:
			void projectOpened(Project *project);
			void recentProjectsChanged(const QStringList &list);


		private:
			void openProject(std::unique_ptr<Project>&& project);

		private:
			QStringList mRecentProjects;
			bool mReloadProject;

			QSettings mSettings;

			Addons::AddonCollector *mAddons;

			std::unique_ptr<Project> mProject;

			
		};

	}
}