#pragma once

#include "Model\Project\ProjectElement.h"
#include "Model\Documents\DocumentStore.h"
#include "ApplicationLauncher.h"

namespace Maditor {
	namespace Model {

		class MADITOR_MODEL_EXPORT ApplicationConfig : public DocumentStore, public ProjectElement {
			Q_OBJECT

		public:
			ApplicationConfig(ConfigList *parent, const QString &name);
			ApplicationConfig(QDomElement data, ConfigList *parent);
			~ApplicationConfig();

			void generateInfo(Shared::ApplicationInfo &appInfo, QWindow *w = nullptr);

			bool hasModuleEnabled(Module *module);

			ApplicationLauncher *createInstace();

		private:
			void init();

		signals:
			void documentCreated(ApplicationLauncher *);
			void documentDestroyed(ApplicationLauncher *);

		private slots:
			void onDocumentCreated(Document *doc);
			void onDocumentDestroyed(Document *doc);

		public:
			// Inherited via ProjectElement
			virtual int childCount() const override;
			virtual TreeItem * child(int i) override;
			virtual Project * project() override;
			virtual QString path() const override;

		private:
			ConfigList *mParent;			

		};

	}
}