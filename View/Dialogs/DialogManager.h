#pragma once


//#include "Model\Project\Generators\ClassGeneratorFactory.h"

#include "Model/DialogManager.h"

namespace Maditor {

	namespace View {
		namespace Dialogs {

			class DialogManager : public Model::DialogManager
			{

			public:
				DialogManager();
				~DialogManager();

				virtual bool confirmFileOverwrite(const QString &filePath, QMessageBox::StandardButton *result = 0) override;

				virtual SettingsDialog *settingsDialog() override;

			public slots:
				virtual void onProjectOpened(Model::Project *project) override;
				virtual void onModuleAdded(Model::Module *module) override;
				virtual void onClassAdded(Model::Generators::ClassGenerator * generator) override;

				virtual void showNewProjectDialog() override;
				virtual void showLoadProjectDialog() override;
				virtual void showNewModuleDialog() override;
				virtual void showNewClassDialog() override;

				virtual void showNewGuiHandlerDialog(Model::Module *module, const QString &name) override;
				virtual void showNewGlobalAPIDialog(Model::Module *module, const QString &name) override;
				virtual void showNewEntityComponentDialog(Model::Module *module, const QString &name) override;
				virtual void showNewSceneComponentDialog(Model::Module *module, const QString &name) override;
				virtual void showNewGameHandlerDialog(Model::Module *module, const QString &name) override;
				virtual void showNewOtherClassDialog(Model::Module *module, const QString &name) override;

				virtual void showModulePropertiesDialog(Model::Module *module) override;

				virtual void showSettingsDialog() override;

				virtual void showDeleteClassDialog() override;

			private:
				SettingsDialog *mSettingsDialog;

			};
		}
	}
}

