#pragma once

#include "Project/Project.h"
#include "MaditorLog.h"
#include "Logs\LogsModel.h"

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
			LogsModel *logs();

			Addons::AddonCollector *addons();

			void setDialogManager(DialogManager *manager);

		public slots:
			void loadProject();
			void newProject();


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

			
			MaditorLog mLog;
			LogsModel mLogs;
			
			std::unique_ptr<Project> mProject;

			DialogManager *mDialogManager;

		};

	}
}