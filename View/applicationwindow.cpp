#include "maditorlib.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Model\maditor.h"

#include "Dialogs\DialogManager.h"

#include "Model\ApplicationLauncher.h"

#include "Model\OgreWindow.h"

#include "ui_mainwindow.h"

namespace Maditor {
namespace View {

	ApplicationWindow::ApplicationWindow(QWidget *parent) :
		QStackedWidget(parent),
		mUi(0){

	}

	void ApplicationWindow::setupUi(Ui::MainWindow * ui, QMainWindow * window)
	{
		mUi = ui;

		QToolBar *toolbar = new QToolBar;
		toolbar->setObjectName("ApplicationToolbar");

		QList<QAction*> result;
		result.push_back(ui->actionInit);
		toolbar->addActions(result);

		window->addToolBar(toolbar);

	}

	void ApplicationWindow::setModel(Model::ApplicationLauncher * app)
	{
		addWidget(QWidget::createWindowContainer(app->window()));

		setCurrentIndex(1);

		mUi->actionInit->setEnabled(true);
		connect(mUi->actionInit, &QAction::triggered, app, &Model::ApplicationLauncher::init);
		connect(app, &Model::ApplicationLauncher::applicationInitializing, this, &ApplicationWindow::onApplicationInitializing);
		connect(app, &Model::ApplicationLauncher::applicationShutdown, this, &ApplicationWindow::onApplicationShutdown);
	}

	void ApplicationWindow::onApplicationShutdown()
	{
		setCurrentIndex(0);
		mUi->actionInit->setEnabled(true);
	}

	void ApplicationWindow::onApplicationInitializing() {
		setCurrentIndex(2);
		mUi->actionInit->setEnabled(false);
	}

} // namespace View
} // namespace Maditor
