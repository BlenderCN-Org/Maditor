#include "maditorviewlib.h"

#include "mainwindow.h"
#include "mainwindow_ui.h"

#include "maditorview.h"



#include "editorsettingswidget.h"

#include "Application\applicationview.h"
#include "Logs\logsview.h"
#include "Project\projectview.h"

namespace Maditor {
namespace View {

	MaditorView::MaditorView() :
		mDialogManager(new Dialogs::DialogManager),
		mApplication(new ApplicationView),
		mLogs(new LogsView),
		mProject(new ProjectView),
		mUi(nullptr){

	}

	MaditorView::~MaditorView()
	{
		delete mDialogManager;
		delete mApplication;
		delete mLogs;
		delete mProject;

	}

	void MaditorView::setupUi(Ui::MainWindow * ui, MainWindow * window)
	{
		mUi = ui;

		mApplication->setupUi(ui, window);
		mLogs->setupUi(ui, window);
		mProject->setupUi(ui, window);


		mRecentProjectInitialActionCount = ui->menuRecentProjects->actions().count();
	
		createToolbar(window, "Projects",
		{
			ui->actionNewProject,
			ui->actionLoadProject
		});

		setConnections({
			{ ui->actionNewProject, &Model::Maditor::newProject },
			{ ui->actionLoadProject, &Model::Maditor::loadProject },
			{ ui->actionClear_List, &Model::Maditor::clearRecentProjects}
		});

		connect(ui->menuRecentProjects, &QMenu::triggered, this, &MaditorView::recentProjectClicked);

		connect(ui->actionSettings, &QAction::triggered, mDialogManager, &Dialogs::DialogManager::showSettingsDialog);

	}

	void MaditorView::setModel(Model::Maditor * model)
	{
		ComponentView::setModel(model);

		mLogs->setModel(model->logs());

		model->setDialogManager(mDialogManager);
		createSettingsTab(mDialogManager, new EditorSettingsWidget(model), "Projects");

		updateRecentProjects(model->recentProjects());

		connect(model, &Model::Maditor::recentProjectsChanged, this, &MaditorView::updateRecentProjects);
		connect(model, &Model::Maditor::projectOpened, this, &MaditorView::onProjectOpened);

		if (model->project()) {
			onProjectOpened(model->project());
		}
	}


	Dialogs::DialogManager * MaditorView::dialogs()
	{
		return mDialogManager;
	}

	bool MaditorView::closeEvent()
	{
		bool yesToAll = false, noToAll = false;
		for (int i = 0; i < mUi->tabWidget->count(); ++i) {
			if (DocumentView *view = dynamic_cast<DocumentView*>(mUi->tabWidget->widget(i))) {
				if (yesToAll) {
					view->save();
				}
				else if (noToAll) {
					view->discardChanges();
				}
				else {
					switch (view->requestClose(true)) {
					case QMessageBox::NoToAll:
						noToAll = true;
						break;
					case QMessageBox::YesToAll:
						yesToAll = true;
						break;
					case QMessageBox::Abort:
						return false;
					default:
						break;
					}
				}
			}
		}
		return true;
	}


	void MaditorView::onProjectOpened(Model::Project *project) {
		mApplication->setConfigModel(project->configList());
		mProject->setModel(project);
	}

	void MaditorView::updateRecentProjects(const QStringList & list)
	{
		QMenu *menu = mUi->menuRecentProjects;
		for (QAction *action : menu->actions().mid(mRecentProjectInitialActionCount)) {
			menu->removeAction(action);
		}

		for (const QString &project : list) {
			menu->addAction(project);
		}
	}



	void MaditorView::recentProjectClicked(QAction * action)
	{
		if (mUi->menuRecentProjects->actions().indexOf(action) >= mRecentProjectInitialActionCount)
			model()->loadProject(action->text());
	}

} // namespace View
} // namespace Maditor
