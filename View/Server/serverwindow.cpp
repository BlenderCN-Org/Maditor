#include "serverlib.h"
#include "serverwindow.h"
#include "ui_serverwindow.h"
#include "Model\Server\ServerLauncher.h"


namespace Maditor {
namespace View {

ServerWindow::ServerWindow(Model::ServerLauncher *model, QTabWidget *tabWidget) :
    ui(new Ui::ServerWindow),
	mModel(model),
	mTabWidget(tabWidget)
{
    ui->setupUi(this);

	ui->modulesWidget->setModel(model->moduleLoader());
	ui->logsWidget->setModel(model->logs());

	connect(model, &Model::ServerLauncher::serverStarting, this, &ServerWindow::onServerStarting);
	connect(model, &Model::ServerLauncher::serverStarted, this, &ServerWindow::onServerStarted);
	connect(model, &Model::ServerLauncher::serverShutdown, this, &ServerWindow::onServerShutdown);

	connect(ui->startButton, &QPushButton::clicked, model, &Model::ServerLauncher::start);
	connect(ui->startNoDebugButton, &QPushButton::clicked, model, &Model::ServerLauncher::startNoDebug);
	connect(ui->shutdownButton, &QPushButton::clicked, model, &Model::ServerLauncher::shutdown);
	connect(ui->killButton, &QPushButton::clicked, model, &Model::ServerLauncher::kill);
}

ServerWindow::~ServerWindow()
{
    delete ui;
}

void ServerWindow::onServerStarting()
{
	mTabWidget->setCurrentWidget(this);
	ui->startButton->setEnabled(false);
	ui->startNoDebugButton->setEnabled(false);
	ui->killButton->setEnabled(true);
}

void ServerWindow::onServerStarted()
{
	ui->shutdownButton->setEnabled(true);
}

void ServerWindow::onServerShutdown()
{
	ui->startButton->setEnabled(true);
	ui->startNoDebugButton->setEnabled(true);
	ui->shutdownButton->setEnabled(false);
	ui->killButton->setEnabled(false);
}

} // namespace View
} // namespace Maditor
