#include "serverlib.h"
#include "serverwindow.h"
#include "ui_serverwindow.h"
#include "Model\Server\ServerLauncher.h"

namespace Maditor {
namespace View {

ServerWindow::ServerWindow(Model::ServerLauncher *model, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServerWindow),
	mModel(model)
{
    ui->setupUi(this);

	ui->modulesWidget->setModel(model->moduleLoader());
	ui->logsWidget->setModel(model->logs());
}

ServerWindow::~ServerWindow()
{
    delete ui;
}

} // namespace View
} // namespace Maditor
