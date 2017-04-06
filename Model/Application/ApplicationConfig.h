#pragma once

#include "Model\Project\ProjectElement.h"

namespace Maditor {
	namespace Model {

		class MADITOR_MODEL_EXPORT ApplicationConfig : public QObject, public ProjectElement{
			Q_OBJECT

		public:
			ApplicationConfig(ConfigList *parent, const QString &name);
			ApplicationConfig(QDomElement data, ConfigList *parent);
			~ApplicationConfig();

			ApplicationLauncher *createInstance();

			void generateInfo(Shared::ApplicationInfo &appInfo, QWindow *w = nullptr);

			bool hasModuleEnabled(Module *module);

		signals:
			void instanceCreated(ApplicationLauncher*);
			void instanceDestroyed(ApplicationLauncher*);

		private:
			void init();

			void destroyInstance(ApplicationLauncher*);

		public:
			// Inherited via ProjectElement
			virtual int childCount() const override;
			virtual TreeItem * child(int i) override;
			virtual Project * project() override;
			virtual QString path() const override;

		private:
			ConfigList *mParent;

			std::list<std::unique_ptr<ApplicationLauncher>> mInstances;

		};

	}
}