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
		mProject(new ProjectView)
	{
		ui->setupUi(this);

		setModel(model);
		setupUi();

		model->setDialogManager(mDialogManager);

		mRecentProjectInitialActionCount = ui->menuRecentProjects->actions().count();

		updateRecentProjects(model->recentProjects());

		createSettingsTab(mDialogManager, new EditorSettingsWidget(model), "Projects");

		setConnections({
			{ui->actionNewProject, &Model::Maditor::newProject},
			{ui->actionLoadProject, &Model::Maditor::loadProject}
		});

		connect(model, &Model::Maditor::recentProjectsChanged, this, &MainWindow::updateRecentProjects);
		connect(ui->menuRecentProjects, &QMenu::triggered, this, &MainWindow::recentProjectClicked);

		connect(ui->actionSettings, &QAction::triggered, mDialogManager, &Dialogs::DialogManager::showSettingsDialog);

		connect(model, &Model::Maditor::projectOpened, this, &MainWindow::onProjectOpened);

		mLogs->setModel(model->logs());

		show();

		QSettings &settings = model->settings();
		settings.beginGroup("Window");
		restoreGeometry(settings.value("geometry").toByteArray());
		restoreState(settings.value("state").toByteArray(), 0);
		settings.endGroup();

		if (model->project()) {
			onProjectOpened(model->project());
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

	QSettings &settings = model()->settings();
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

void MainWindow::setupUi()
{
	mApplication->setupUi(ui, this);
	mLogs->setupUi(ui, this);
	mProject->setupUi(ui, this);
	model()->addons()->setupUi(ui, this);
}

void MainWindow::onProjectOpened(Model::Project *project) {
	mApplication->setConfigModel(project->configList());
	mProject->setModel(project);
}

void MainWindow::clearRecentProjects()
{
	model()->clearRecentProjects();
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
		model()->loadProject(action->text());
}


} // namespace View
} // namespace Maditor
