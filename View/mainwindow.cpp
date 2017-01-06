#include "maditorviewlib.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Model\maditor.h"

#include "Dialogs\DialogManager.h"
#include "Dialogs\settingsdialog.h"

#include "editorsettingswidget.h"

#include "Model\Addons\Addon.h"

#include "Application\applicationview.h"
#include "Logs\logsview.h"
#include "Project\projectview.h"

namespace Maditor {
namespace View {

	MainWindow::MainWindow(Model::Maditor *model) :
		ui(new Ui::MainWindow),
		mDialogManager(new Dialogs::DialogManager),
		mApplication(new ApplicationView),
		mLogs(new LogsView),
		mProject(new ProjectView),
		mEditorSettingsWidget(new EditorSettingsWidget(model)),
		mModel(model)
{
    ui->setupUi(this);


	mRecentProjectInitialActionCount = ui->menuRecentProjects->actions().count();

	updateRecentProjects(mModel->recentProjects());

	mDialogManager->settingsDialog()->addSettingsTab(mEditorSettingsWidget, "Projects");

	connect(mModel, &Model::Maditor::recentProjectsChanged, this, &MainWindow::updateRecentProjects);
	connect(ui->menuRecentProjects, &QMenu::triggered, this, &MainWindow::recentProjectClicked);

	mApplication->setupUi(ui, this);

	connect(ui->actionNewProject, &QAction::triggered, mDialogManager, &Dialogs::DialogManager::showNewProjectDialog);
	connect(ui->actionLoadProject, &QAction::triggered, mDialogManager, &Dialogs::DialogManager::showLoadProjectDialog);

	connect(ui->actionSettings, &QAction::triggered, mDialogManager, &Dialogs::DialogManager::showSettingsDialog);

	connect(mDialogManager, &Dialogs::DialogManager::newProjectDialogAccepted, model, &Model::Maditor::newProject);
	connect(mDialogManager, &Dialogs::DialogManager::loadProjectDialogAccepted, model, &Model::Maditor::loadProject);

	connect(model, &Model::Maditor::projectOpened, this, &MainWindow::onProjectOpened);

	mLogs->setupUi(ui, this);

	mLogs->setModel(mModel->logs());

	mProject->setupUi(ui, this);

	model->addons()->setupUi(ui, this);

	QSettings &settings = mModel->settings();
	settings.beginGroup("Window");
	restoreGeometry(settings.value("geometry").toByteArray());
	restoreState(settings.value("state").toByteArray(), 0);
	settings.endGroup();

	if (mModel->project()) {
		onProjectOpened(mModel->project());
	}

}

MainWindow::~MainWindow()
{
    delete ui;

	delete mApplication;
	delete mLogs;
	delete mProject;
	delete mDialogManager;
}

void MainWindow::closeEvent(QCloseEvent *event)
{

	QSettings &settings = mModel->settings();
	settings.beginGroup("Window");
	settings.setValue("geometry", saveGeometry());
	settings.setValue("state", saveState(0));
	settings.endGroup();

	event->accept();
}

Dialogs::DialogManager * MainWindow::dialogs()
{
	return mDialogManager;
}

void MainWindow::onProjectOpened(Model::Project *project) {
	mApplication->setModel(project->application());
	mProject->setModel(project);
}

void MainWindow::clearRecentProjects()
{
	mModel->clearRecentProjects();
}

void MainWindow::updateRecentProjects(const QStringList & list)
{
	QMenu *menu = ui->menuRecentProjects;
	for (QAction *action : menu->actions().mid(mRecentProjectInitialActionCount)) {
		menu->removeAction(action);
	}

	for (const QString &project : list) {
		menu->addAction(project);
	}
}

void MainWindow::recentProjectClicked(QAction * action)
{
	if (ui->menuRecentProjects->actions().indexOf(action) >= mRecentProjectInitialActionCount)
		mModel->loadProject(action->text());
}


} // namespace View
} // namespace Maditor
