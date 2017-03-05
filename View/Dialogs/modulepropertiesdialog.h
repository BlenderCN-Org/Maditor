#ifndef MODULEPROPERTIESDIALOG_H
#define MODULEPROPERTIESDIALOG_H


namespace Maditor {
	namespace View {
		namespace Dialogs {

			namespace Ui {
				class ModulePropertiesDialog;
			}

class ModulePropertiesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ModulePropertiesDialog(Model::ModuleList *list);
    ~ModulePropertiesDialog();

public slots:
	bool apply();

private:
	Model::ModuleList *mModules;
	std::list<ModulePropertiesWidget*> mModuleWidgets;

    Ui::ModulePropertiesDialog *ui;
};


		}
	}
}

#endif // MODULEPROPERTIESDIALOG_H
