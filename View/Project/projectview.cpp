#include "maditorviewlib.h"

#include "mainwindow.h"
#include "mainwindow_ui.h"

#include "projectview.h"





namespace Maditor {
namespace View {

	ProjectView::ProjectView() :
		mUi(0){

	}

	void ProjectView::setupUi(Ui::MainWindow * ui, QMainWindow * window)
	{
		mUi = ui;

		/*createToolbar(window, "ApplicationToolbar", {
			{ ui->actionInit, &Model::Project::init },
			{ ui->actionInit_No_Debug, &Model::ApplicationLauncher::initNoDebug },
			{ ui->actionFinalize, &Model::ApplicationLauncher::finalize },
			{ ui->actionStart, &Model::ApplicationLauncher::start },
			{ ui->actionPause, &Model::ApplicationLauncher::pause },
			{ ui->actionStop, &Model::ApplicationLauncher::stop },
		})*/;

	}

	void ProjectView::setModel(Model::Project * project)
	{
		ComponentView::setModel(project);

		mUi->projectWidget->setModel(project);
		mUi->mediaWidget->setModel(project->getMedia());

	}

} // namespace View
} // namespace Maditor
