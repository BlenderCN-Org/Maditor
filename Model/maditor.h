#pragma once

#include "Project/Project.h"

namespace Maditor {
	namespace Model {

		class MADITOR_MODEL_EXPORT Maditor : public QObject {
			Q_OBJECT

		public:
			Maditor();

			void newProject(const QString &path, const QString &name);
			void loadProject(const QString &path);


		signals:
			void projectOpened(Project *project);

		private:
			void openProject(std::unique_ptr<Project>&& project);

		private:
			

			std::unique_ptr<Project> mProject;
		};

	}
}