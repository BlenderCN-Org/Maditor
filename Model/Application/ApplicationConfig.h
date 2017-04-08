#pragma once

#include "Model\Project\ProjectElement.h"
#include "Model\Documents\DocumentStore.h"
#include "ApplicationLauncher.h"

namespace Maditor {
	namespace Model {

		class MADITOR_MODEL_EXPORT ApplicationConfig : public Document, public ProjectElement {
			Q_OBJECT

		public:
			enum Launcher {
				MADITOR_LAUNCHER = 0,
				CUSTOM_LAUNCHER = 1
			};

			enum LauncherType {
				CLIENT_LAUNCHER = 0,
				SERVER_LAUNCHER = 1
			};

		public:
			ApplicationConfig(ConfigList *parent, const QString &name);
			ApplicationConfig(QDomElement data, ConfigList *parent);
			~ApplicationConfig();

			void generateInfo(Shared::ApplicationInfo &appInfo, QWindow *w = nullptr);

			bool hasModuleEnabled(Module *module);

			ApplicationLauncher *createInstace();

			Launcher launcher();
			const QString &customExecutableCmd();
			LauncherType launcherType();
			Generators::ServerClassGenerator *server();

			virtual void save() override;
			virtual void discardChanges() override;

		public slots:
			void setLauncher(Launcher launcher);
			void setLauncherType(LauncherType type);
			void setCustomExecutableCmd(const QString &cmd);
			void setServer(Generators::ServerClassGenerator *server);
			void setServerByName(const QString &name);

		protected:
			virtual void doubleClicked() override;

			virtual bool storeData() override;
			virtual void restoreData() override;

		private:
			void init();

		signals:
			void documentCreated(ApplicationLauncher *);
			void documentDestroyed(ApplicationLauncher *);

			void launcherChanged(Launcher);
			void launcherTypeChanged(LauncherType);
			void customExecutableCmdChanged(const QString &);
			void serverChanged(Generators::ServerClassGenerator*);

		private slots:
			void onDocumentCreated(Document *doc);
			void onDocumentDestroyed(Document *doc);

		public:
			// Inherited via ProjectElement
			virtual int childCount() const override;
			virtual ProjectElement * child(int i) override;
			virtual Project * project() override;
			virtual QString path() const override;

		private:
			ConfigList *mParent;
			DocumentStore mDocuments;


			Launcher mLauncher;
			QString mCustomExecutableCmd;

			LauncherType mLauncherType;
			Generators::ServerClassGenerator *mServer;



		};

	}
}