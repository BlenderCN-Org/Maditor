#include "maditorviewlib.h"

#include "mainwindow.h"
#include "mainwindow_ui.h"

#include "logsview.h"


namespace Maditor {
namespace View {

	LogsView::LogsView() :
		mUi(nullptr){

	}

	void LogsView::setupUi(Ui::MainWindow * ui, QMainWindow * window)
	{
		mUi = ui;

		/*createToolbar(window, "ApplicationToolbar", {
			{ ui->actionInit, &Model::ApplicationLauncher::init },
			{ ui->actionInit_No_Debug, &Model::ApplicationLauncher::initNoDebug },
			{ ui->actionFinalize, &Model::ApplicationLauncher::finalize },
			{ ui->actionStart, &Model::ApplicationLauncher::start },
			{ ui->actionPause, &Model::ApplicationLauncher::pause },
			{ ui->actionStop, &Model::ApplicationLauncher::stop },
		});*/

	}

	void LogsView::setModel(Model::LogTableModel * logs)
	{
		ComponentView::setModel(logs);

		mUi->logsWidget->setModel(logs);

	}


} // namespace View
} // namespace Maditor
