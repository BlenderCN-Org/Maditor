#include "modulepropertieswidget.h"
#include "modulepropertieswidget.h"
#include "ui_modulepropertieswidget.h"

#include "Model\Project\ModuleList.h"

namespace Maditor {
namespace View {

ModulePropertiesWidget::ModulePropertiesWidget(Model::Module *module) :
    QWidget(),
	mModule(module),
    ui(new Ui::ModulePropertiesWidget)
{
    ui->setupUi(this);

	for (const std::unique_ptr<Model::Module> &dep : *module->parent()) {
		QListWidgetItem *item = new QListWidgetItem(dep->name(), ui->dependenciesList);
		item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
		item->setCheckState(module->dependencies().contains(dep->name()) ? Qt::Checked : Qt::Unchecked);
	}

	ui->dependenciesList->sortItems();

}

ModulePropertiesWidget::~ModulePropertiesWidget()
{
    delete ui;
}

bool ModulePropertiesWidget::apply()
{
	bool valid = true;

	for (int i = 0; i < ui->dependenciesList->count(); ++i) {
		QListWidgetItem *item = ui->dependenciesList->item(i);
		bool isChecked = item->checkState() == Qt::Checked;
		bool isDep = mModule->dependencies().contains(item->text());
		if (isChecked != isDep) {
			if (!isChecked) {
				mModule->removeDependency(item->text());
			}
			else {
				if (!mModule->addDependency(item->text())) {
					QMessageBox::critical(0, "Cannot set Dependencies!",
						QString("Dependency to '%1' can not be set to Module '%2'").arg(item->text(), mModule->name()));
					valid = false;
				}
			}
		}
	}

	return valid;
}

} // namespace View
} // namespace Maditor
