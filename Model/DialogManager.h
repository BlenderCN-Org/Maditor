#pragma once


#include "Model\Project\Generators\ClassGeneratorFactory.h"


namespace Maditor {

	namespace Model {


			class MADITOR_MODEL_EXPORT DialogManager : public QObject
			{
				Q_OBJECT

			public:
				DialogManager() {
					sSingleton = this;
				};
				~DialogManager() = default;

				virtual bool confirmFileOverwrite(const QString &filePath, QMessageBox::StandardButton *result = 0) = 0;
				static bool confirmFileOverwriteStatic(const QString &filePath, QMessageBox::StandardButton *result = 0) {
					return sSingleton->confirmFileOverwrite(filePath, result);
				}

				virtual View::Dialogs::SettingsDialog *settingsDialog() = 0;
					

			signals:
				void newProjectDialogAccepted(const QString &path, const QString &name);
				void loadProjectDialogAccepted(const QString &path);
				void newModuleDialogAccepted(const QString &name);
				void newClassDialogAccepted(Model::Module *module, const QString &name, Model::Generators::ClassGeneratorFactory::ClassType type);

				void newGuiHandlerDialogAccepted(Model::Module *module, const QString &name, const QString &window, int type, bool hasLayoutFile);
				void newGlobalAPIDialogAccepted(Model::Module *module, const QString &name);
				void newEntityComponentDialogAccepted(Model::Module *module, const QString &name, const QString &componentName);
				void newSceneComponentDialogAccepted(Model::Module *module, const QString &name);
				void newGameHandlerDialogAccepted(Model::Module *module, const QString &name);
				void newOtherClassDialogAccepted(Model::Module *module, const QString &name);

				void deleteClassDialogAccepted(Model::Generators::ClassGenerator *generator, bool deleteFiles);

			public slots:
				virtual void onProjectOpened(Model::Project *project) = 0;
				virtual void onModuleAdded(Model::Module *module) = 0;
				virtual void onClassAdded(Model::Generators::ClassGenerator * generator) = 0;

				virtual void showNewProjectDialog() = 0;
				virtual void showLoadProjectDialog() = 0;
				virtual void showNewModuleDialog() = 0;
				virtual void showNewClassDialog() = 0;

				virtual void showNewGuiHandlerDialog(Model::Module *module, const QString &name) = 0;
				virtual void showNewGlobalAPIDialog(Model::Module *module, const QString &name) = 0;
				virtual void showNewEntityComponentDialog(Model::Module *module, const QString &name) = 0;
				virtual void showNewSceneComponentDialog(Model::Module *module, const QString &name) = 0;
				virtual void showNewGameHandlerDialog(Model::Module *module, const QString &name) = 0;
				virtual void showNewOtherClassDialog(Model::Module *module, const QString &name) = 0;

				virtual void showModulePropertiesDialog(Model::Module *module) = 0;

				virtual void showSettingsDialog() = 0;

				virtual void showDeleteClassDialog() = 0;

			private:
				static DialogManager *sSingleton;

				
			};
	}
}

