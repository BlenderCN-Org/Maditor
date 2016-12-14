#include "maditorlib.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Model\maditor.h"

#include "Dialogs\DialogManager.h"

namespace Maditor {
namespace View {

MainWindow::MainWindow(Model::Maditor *model) :
    ui(new Ui::MainWindow),
	mDialogManager(new Dialogs::DialogManager)
{
    ui->setupUi(this);

	ui->game->setupUi(ui, this);

	connect(ui->actionNewProject, &QAction::triggered, mDialogManager, &Dialogs::DialogManager::showNewProjectDialog);
	connect(ui->actionLoadProject, &QAction::triggered, mDialogManager, &Dialogs::DialogManager::showLoadProjectDialog);

	connect(mDialogManager, &Dialogs::DialogManager::newProjectDialogAccepted, model, &Model::Maditor::newProject);
	connect(mDialogManager, &Dialogs::DialogManager::loadProjectDialogAccepted, model, &Model::Maditor::loadProject);

	connect(model, &Model::Maditor::projectOpened, this, &MainWindow::onProjectOpened);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onProjectOpened(Model::Project *project) {
	ui->game->setModel(project->application());
}

} // namespace View
} // namespace Maditor
