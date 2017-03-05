#ifndef MADITOR_VIEW_MODULEPROPERTIESWIDGET_H
#define MADITOR_VIEW_MODULEPROPERTIESWIDGET_H

#include <QWidget>

namespace Maditor {
namespace View {

namespace Ui {
class ModulePropertiesWidget;
}

class ModulePropertiesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ModulePropertiesWidget(Model::Module *module);
    ~ModulePropertiesWidget();

	bool apply();

private:
    Ui::ModulePropertiesWidget *ui;

	Model::Module *mModule;
};


} // namespace View
} // namespace Maditor
#endif // MADITOR_VIEW_MODULEPROPERTIESWIDGET_H
