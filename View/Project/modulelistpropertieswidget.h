#ifndef MODULEPROPERTIESDIALOG_H
#define MODULEPROPERTIESDIALOG_H


namespace Maditor {
	namespace View {

			namespace Ui {
				class ModuleListPropertiesWidget;
			}

class ModuleListPropertiesWidget : public QDialog
{
    Q_OBJECT

public:
    explicit ModuleListPropertiesWidget(Model::ModuleList *list);
    ~ModuleListPropertiesWidget();

public slots:
	bool apply();

private:
	Model::ModuleList *mModules;
	std::list<ModulePropertiesWidget*> mModuleWidgets;

    Ui::ModuleListPropertiesWidget *ui;
};


	}
}

#endif // MODULEPROPERTIESDIALOG_H
