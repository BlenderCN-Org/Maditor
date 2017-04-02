#include "maditorviewlib.h"

#include "mainwindow.h"
#include "mainwindow_ui.h"

#include "applicationview.h"





namespace Maditor {
namespace View {

	ApplicationView::ApplicationView() :
		mUi(nullptr){

	}

	void ApplicationView::setupUi(Ui::MainWindow * ui, MainWindow * window)
	{
		mUi = ui;

		setConnections({
			{ ui->actionSetup, &Model::ApplicationLauncher::setup },
			{ ui->actionSetup_No_Debug, &Model::ApplicationLauncher::setupNoDebug },
			{ ui->actionShutdown, &Model::ApplicationLauncher::shutdown },
			{ ui->actionKill, &Model::ApplicationLauncher::kill},
			{ ui->actionStart, &Model::ApplicationLauncher::start },
			{ ui->actionPause, &Model::ApplicationLauncher::pause },
			{ ui->actionStop, &Model::ApplicationLauncher::stop },
		});

		createToolbar(window, "ApplicationToolbar", {
			ui->actionSetup,
			ui->actionSetup_No_Debug,
			ui->actionShutdown,
			ui->actionKill,
			ui->actionStart,
			ui->actionPause,
			ui->actionStop
		});


	}

	void ApplicationView::setModel(Model::ApplicationLauncher * app)
	{
		ComponentView::setModel(app);

		mUi->game->setModel(app);
		mUi->modulesWidget->setModel(app->moduleLoader());
		mUi->performanceWidget->setModel(app->util()->profiler());
		mUi->appStatsWidget->setModel(app->util()->stats());

		mUi->actionSetup->setEnabled(true);
		connect(app, &Model::ApplicationLauncher::applicationSettingup, this, &ApplicationView::onApplicationSettingup);
		connect(app, &Model::ApplicationLauncher::applicationSetup, this, &ApplicationView::onApplicationSetup);
		connect(app, &Model::ApplicationLauncher::applicationStarted, this, &ApplicationView::onApplicationStarted);
		connect(app, &Model::ApplicationLauncher::applicationStopped, this, &ApplicationView::onApplicationStopped);
		connect(app, &Model::ApplicationLauncher::applicationShutdown, this, &ApplicationView::onApplicationShutdown);
	}

	void ApplicationView::onApplicationSettingup() {
		mUi->game->setCurrentIndex(2);
		mUi->actionSetup->setEnabled(false);
		mUi->actionSetup_No_Debug->setEnabled(false);
		mUi->actionKill->setEnabled(true);
	}

	void ApplicationView::onApplicationSetup()
	{
		mUi->game->setCurrentIndex(1);
		mUi->actionStart->setEnabled(true);
		mUi->actionShutdown->setEnabled(true);		
	}

	void ApplicationView::onApplicationStarted() {
		mUi->game->setCurrentIndex(2);
		mUi->actionStart->setEnabled(false);
		mUi->actionPause->setEnabled(true);
		mUi->actionStop->setEnabled(true);
	}

	void ApplicationView::onApplicationStopped()
	{
		mUi->game->setCurrentIndex(1);
		mUi->actionStart->setEnabled(true);
		mUi->actionPause->setEnabled(false);
		mUi->actionStop->setEnabled(false);
	}

	void ApplicationView::onApplicationShutdown()
	{
		mUi->game->setCurrentIndex(0);
		mUi->actionSetup->setEnabled(true);
		mUi->actionSetup_No_Debug->setEnabled(true);
		mUi->actionShutdown->setEnabled(false);
		mUi->actionKill->setEnabled(false);
		mUi->actionStart->setEnabled(false);
		mUi->actionStop->setEnabled(false);
		mUi->actionPause->setEnabled(false);
	}


} // namespace View
} // namespace Maditor
