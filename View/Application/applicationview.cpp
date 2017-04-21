#include "maditorviewlib.h"

#include "mainwindow.h"
#include "mainwindow_ui.h"

#include "applicationview.h"

#include "Model\Project\ConfigList.h"

#include "applicationwindow.h"


namespace Maditor {
namespace View {

	ApplicationView::ApplicationView() :
		mUi(nullptr),
		mList(nullptr),
		mCurrentWidget(nullptr){

	}

	void ApplicationView::setupUi(Ui::MainWindow * ui, MainWindow * window)
	{
		WindowSpawner<Model::ApplicationLauncher, ApplicationWindow>::setupUi(ui);
		WindowSpawner<Model::ApplicationLauncher, ApplicationLog>::setupUi(ui);

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

		createToolbar(window, "Application", {
			mCurrentConfigSelector.menuAction(),
			ui->actionSetup,
			ui->actionSetup_No_Debug,
			ui->actionShutdown,
			ui->actionKill,
			ui->actionStart,
			ui->actionPause,
			ui->actionStop
		});

		connect(&mCurrentConfigSelector, &QMenu::triggered, this, &ApplicationView::selectConfig);
		connect(mCurrentConfigSelector.menuAction(), &QAction::triggered, this, &ApplicationView::createCurrentConfig);

	}

	void ApplicationView::setConfigModel(Model::ConfigList * list)
	{
		mList = list;

		mCurrentConfigSelector.clear();
		for (const QString &action : list->getConfigs()) {
			mCurrentConfigSelector.addAction(action);
		}		

		if (mCurrentConfigSelector.actions().size() > 0) {
			mCurrentConfigSelector.setEnabled(true);
			mCurrentConfigSelector.setTitle(QString("New %1").arg(mCurrentConfigSelector.actions().front()->text()));
		}
		else {
			mCurrentConfigSelector.setEnabled(false);
		}

		connect(list, &Model::ConfigList::instanceAdded, this, &ApplicationView::onInstanceAdded);
		connect(list, &Model::ConfigList::instanceDestroyed, this, &ApplicationView::onInstanceDestroyed);
	}

	void ApplicationView::setModel(Model::ApplicationLauncher * app)
	{
		ComponentView::setModel(app);

		for (QMetaObject::Connection &conn : mAppConnections)
			disconnect(conn);

		
		mUi->modulesWidget->setModel(app->moduleLoader());
		mUi->performanceWidget->setModel(app->util()->profiler());
		mUi->appStatsWidget->setModel(app->util()->stats());

		mAppConnections.emplace_back(connect(app, &Model::ApplicationLauncher::applicationSettingup, this, &ApplicationView::onApplicationSettingup));
		mAppConnections.emplace_back(connect(app, &Model::ApplicationLauncher::applicationSetup, this, &ApplicationView::onApplicationSetup));
		mAppConnections.emplace_back(connect(app, &Model::ApplicationLauncher::applicationStarted, this, &ApplicationView::onApplicationStarted));
		mAppConnections.emplace_back(connect(app, &Model::ApplicationLauncher::applicationStopped, this, &ApplicationView::onApplicationStopped));
		mAppConnections.emplace_back(connect(app, &Model::ApplicationLauncher::applicationShutdown, this, &ApplicationView::onApplicationShutdown));

		mUi->actionSetup->setEnabled(!app->isLaunched());
		mUi->actionSetup_No_Debug->setEnabled(!app->isLaunched());
		mUi->actionShutdown->setEnabled(app->isSetup() && app->isLauncher());
		mUi->actionKill->setEnabled(app->isLaunched());
		mUi->actionStart->setEnabled(!app->isRunning() && app->isSetup() && app->isClient());
		mUi->actionStop->setEnabled(app->isRunning() && app->isClient());
		mUi->actionPause->setEnabled(app->isRunning() && app->isClient());
	}

	void ApplicationView::clearModel()
	{
		ComponentView::clearModel();

		for (QMetaObject::Connection &conn : mAppConnections)
			disconnect(conn);

		mUi->modulesWidget->setModel(nullptr);
		mUi->appStatsWidget->clearModel();

		mUi->actionSetup->setEnabled(false);
		mUi->actionSetup_No_Debug->setEnabled(false);
		mUi->actionShutdown->setEnabled(false);
		mUi->actionKill->setEnabled(false);
		mUi->actionStart->setEnabled(false);
		mUi->actionStop->setEnabled(false);
		mUi->actionPause->setEnabled(false);
	}

	void ApplicationView::currentTabSet(ApplicationWindow * win)
	{
		setModel(win->app());
		mCurrentWidget = win;
	}

	void ApplicationView::currentTabSet(ApplicationLog * win)
	{
		setModel(win->app());
		mCurrentWidget = win;
	}

	void ApplicationView::currentTabCleared(QWidget * w)
	{
		if (mCurrentWidget != w) {
			clearModel();
		}
	}

	void ApplicationView::selectConfig(QAction *action)
	{
		selectConfigName(action->text());
	}

	void ApplicationView::selectConfigName(const QString &name)
	{
		Model::ApplicationConfig *config = mList->getConfig(name);
		assert(config);
		setModel(config->createInstace());
	}

	void ApplicationView::onConfigAdded(Model::ApplicationConfig *config) {
		mCurrentConfigSelector.addAction(config->name());
		if (!mCurrentConfigSelector.isEnabled()) {
			mCurrentConfigSelector.setEnabled(true);
			mCurrentConfigSelector.setTitle(QString("New %1").arg(config->name()));
		}
	}

	void ApplicationView::onInstanceAdded(Model::ApplicationLauncher * instance)
	{
		if (instance->isClient())
			WindowSpawner<Model::ApplicationLauncher, ApplicationWindow>::spawn(instance);
		else
			WindowSpawner<Model::ApplicationLauncher, ApplicationLog>::spawn(instance);
	}

	void ApplicationView::onInstanceDestroyed(Model::ApplicationLauncher * instance)
	{
		if (instance->isClient())
			WindowSpawner<Model::ApplicationLauncher, ApplicationWindow>::remove(instance);
		else
			WindowSpawner<Model::ApplicationLauncher, ApplicationLog>::remove(instance);
	}

	void ApplicationView::createCurrentConfig()
	{
		selectConfigName(mCurrentConfigSelector.title().mid(4));
	}

	void ApplicationView::onApplicationSettingup() {		
		mUi->actionSetup->setEnabled(false);
		mUi->actionSetup_No_Debug->setEnabled(false);
		mUi->actionKill->setEnabled(true);
	}

	void ApplicationView::onApplicationSetup()
	{		
		mUi->actionStart->setEnabled(model()->isClient());
		mUi->actionShutdown->setEnabled(model()->isLauncher());		
	}

	void ApplicationView::onApplicationStarted() {		
		mUi->actionStart->setEnabled(false);
		mUi->actionPause->setEnabled(true);
		mUi->actionStop->setEnabled(true);
	}

	void ApplicationView::onApplicationStopped()
	{		
		mUi->actionStart->setEnabled(true);
		mUi->actionPause->setEnabled(false);
		mUi->actionStop->setEnabled(false);
	}

	void ApplicationView::onApplicationShutdown()
	{		
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
