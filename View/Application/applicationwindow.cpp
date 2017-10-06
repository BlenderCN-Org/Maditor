#include "maditorviewlib.h"


#include "applicationwindow.h"

#include "Model\Application\ApplicationLauncher.h"

#include "Model\Application\OgreWindow.h"

#include "applicationlog.h"

#include "Documents/CentralWidget.h"
#include "Documents/DocumentTabWidget.h"


namespace Maditor {
namespace View {

	ApplicationContainerWindow::ApplicationContainerWindow(Model::ApplicationLauncher *app) :
		QMainWindow(nullptr),
		DocumentView(app, this),
		ComponentView(app),
		mApp(app),
		mAppWindow(nullptr){


		connect(app, &Model::ApplicationLauncher::applicationSettingup, this, &ApplicationContainerWindow::onApplicationSettingup);
		connect(app, &Model::ApplicationLauncher::applicationSetup, this, &ApplicationContainerWindow::onApplicationSetup);
		connect(app, &Model::ApplicationLauncher::applicationStarted, this, &ApplicationContainerWindow::onApplicationStarted);
		connect(app, &Model::ApplicationLauncher::applicationStopped, this, &ApplicationContainerWindow::onApplicationStopped);
		connect(app, &Model::ApplicationLauncher::applicationShutdown, this, &ApplicationContainerWindow::onApplicationShutdown);

		

		actionSetup = new QAction();
		actionSetup->setObjectName(QStringLiteral("actionSetup"));
		actionSetup->setEnabled(false);
		actionSetup->setText(QApplication::translate("Maditor::View::MainWindow", "Setup", 0));
		actionStart = new QAction();
		actionStart->setObjectName(QStringLiteral("actionStart"));
		actionStart->setEnabled(false);
		QIcon icon;
		icon.addFile(QStringLiteral(":/icons/icons/start.jpg"), QSize(), QIcon::Normal, QIcon::Off);
		actionStart->setIcon(icon);
		actionPause = new QAction();
		actionPause->setObjectName(QStringLiteral("actionPause"));
		actionPause->setEnabled(false);
		QIcon icon1;
		icon1.addFile(QStringLiteral(":/icons/icons/pause.png"), QSize(), QIcon::Normal, QIcon::Off);
		actionPause->setIcon(icon1);
		actionStop = new QAction();
		actionStop->setObjectName(QStringLiteral("actionStop"));
		actionStop->setEnabled(false);
		QIcon icon2;
		icon2.addFile(QStringLiteral(":/icons/icons/Stop.jpg"), QSize(), QIcon::Normal, QIcon::Off);
		actionStop->setIcon(icon2);
		actionSetup_No_Debug = new QAction();
		actionSetup_No_Debug->setObjectName(QStringLiteral("actionSetup_No_Debug"));
		actionSetup_No_Debug->setEnabled(false);
		actionSetup_No_Debug->setText(QApplication::translate("Maditor::View::MainWindow", "Setup (No Debug)", 0));
		actionShutdown = new QAction();
		actionShutdown->setObjectName(QStringLiteral("actionShutdown"));
		actionShutdown->setEnabled(false);
		actionShutdown->setText(QApplication::translate("Maditor::View::MainWindow", "Shutdown", 0));
		actionKill = new QAction();
		actionKill->setObjectName(QStringLiteral("actionKill"));
		actionKill->setEnabled(false);
		actionKill->setText(QApplication::translate("Maditor::View::MainWindow", "Kill", 0));

		setConnections({
			{ actionSetup, &Model::ApplicationLauncher::setup },
			{ actionSetup_No_Debug, &Model::ApplicationLauncher::setupNoDebug },
			{ actionShutdown, &Model::ApplicationLauncher::shutdown },
			{ actionKill, &Model::ApplicationLauncher::kill },
			{ actionStart, &Model::ApplicationLauncher::start },
			{ actionPause, &Model::ApplicationLauncher::pause },
			{ actionStop, &Model::ApplicationLauncher::stop },
		});


		connect(app, &Model::ApplicationLauncher::applicationSettingup, this, &ApplicationContainerWindow::onApplicationSettingup);
		connect(app, &Model::ApplicationLauncher::applicationSetup, this, &ApplicationContainerWindow::onApplicationSetup);
		connect(app, &Model::ApplicationLauncher::applicationStarted, this, &ApplicationContainerWindow::onApplicationStarted);
		connect(app, &Model::ApplicationLauncher::applicationStopped, this, &ApplicationContainerWindow::onApplicationStopped);
		connect(app, &Model::ApplicationLauncher::applicationShutdown, this, &ApplicationContainerWindow::onApplicationShutdown);

		actionSetup->setEnabled(!app->isLaunched());
		actionSetup_No_Debug->setEnabled(!app->isLaunched());
		actionShutdown->setEnabled(app->isSetup() && app->isLauncher());
		actionKill->setEnabled(app->isLaunched());
		actionStart->setEnabled(!app->isRunning() && app->isSetup() && app->isClient());
		actionStop->setEnabled(app->isRunning() && app->isClient());
		actionPause->setEnabled(app->isRunning() && app->isClient());

		if (app->isClient()) {

			createToolbar("Application", {
				actionSetup,
				actionSetup_No_Debug,
				actionShutdown,
				actionKill,
				actionStart,
				actionPause,
				actionStop
			});

			mAppWindow = new ApplicationWindow(app);

			setContextMenuPolicy(Qt::ActionsContextMenu);
			QAction *toggleAction = new QAction("Toggle Log");
			addAction(toggleAction);
			connect(toggleAction, &QAction::triggered, mAppWindow, &ApplicationWindow::toggleLog);
			connect(mAppWindow, &ApplicationWindow::clicked, this, &ApplicationContainerWindow::clicked);

			setCentralWidget(mAppWindow);

		}
		else {
			createToolbar("Application", {
				actionSetup,
				actionSetup_No_Debug,
				actionShutdown,
				actionKill
			});

			ApplicationLog *log = new ApplicationLog(app);

			setCentralWidget(log);

		}

		addItemsToWindow(this);

	}

	void ApplicationContainerWindow::clicked()
	{
		CentralWidget::parentTabWidget(this)->focus();
	}

	Model::ApplicationLauncher * ApplicationContainerWindow::app()
	{
		return mApp;
	}

	void ApplicationContainerWindow::clearModel() {
		actionSetup->setEnabled(false);
		actionSetup_No_Debug->setEnabled(false);
		actionShutdown->setEnabled(false);
		actionKill->setEnabled(false);
		actionStart->setEnabled(false);
		actionStop->setEnabled(false);
		actionPause->setEnabled(false);
	}

	void ApplicationContainerWindow::onApplicationSettingup()
	{
		actionSetup->setEnabled(false);
		actionSetup_No_Debug->setEnabled(false);
		actionKill->setEnabled(true);
		if (mAppWindow && mAppWindow->currentIndex() != 2)
			mAppWindow->setCurrentIndex(1);
	}

	void ApplicationContainerWindow::onApplicationSetup()
	{
		actionStart->setEnabled(model()->isClient());
		actionShutdown->setEnabled(model()->isLauncher());
		if (mAppWindow && mAppWindow->currentIndex() != 2)
			mAppWindow->setCurrentIndex(0);
	}

	void ApplicationContainerWindow::onApplicationStarted()
	{
		actionStart->setEnabled(false);
		actionPause->setEnabled(true);
		actionStop->setEnabled(true);
		if (mAppWindow && mAppWindow->currentIndex() != 2)
			mAppWindow->setCurrentIndex(1);
	}

	void ApplicationContainerWindow::onApplicationStopped()
	{
		actionStart->setEnabled(true);
		actionPause->setEnabled(false);
		actionStop->setEnabled(false);
		if (mAppWindow && mAppWindow->currentIndex() != 2)
			mAppWindow->setCurrentIndex(0);
	}

	void ApplicationContainerWindow::onApplicationShutdown()
	{
		actionSetup->setEnabled(true);
		actionSetup_No_Debug->setEnabled(true);
		actionShutdown->setEnabled(false);
		actionKill->setEnabled(false);
		actionStart->setEnabled(false);
		actionStop->setEnabled(false);
		actionPause->setEnabled(false);
		if (mAppWindow && mAppWindow->currentIndex() != 2)
			mAppWindow->setCurrentIndex(0);
	}

	ApplicationWindow::ApplicationWindow(Model::ApplicationLauncher *app) :
		mApp(app)		
	{

		QPalette palette;
		QBrush brush(QColor(197, 197, 197, 255));
		brush.setStyle(Qt::SolidPattern);
		palette.setBrush(QPalette::Active, QPalette::Base, brush);
		QBrush brush1(QColor(177, 177, 177, 255));
		brush1.setStyle(Qt::SolidPattern);
		palette.setBrush(QPalette::Active, QPalette::Window, brush1);
		palette.setBrush(QPalette::Inactive, QPalette::Base, brush);
		palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
		palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
		palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
		setPalette(palette);
		setAutoFillBackground(true);
		

		QWidget *notStarted = new QWidget();
		notStarted->setObjectName(QStringLiteral("notStarted"));
		QGridLayout *gridLayout = new QGridLayout(notStarted);
		gridLayout->setObjectName(QStringLiteral("gridLayout"));
		QLabel *notStartedLabel = new QLabel(notStarted);
		notStartedLabel->setText("The Application is not running!");
		notStartedLabel->setObjectName(QStringLiteral("notStartedLabel"));
		notStartedLabel->setAlignment(Qt::AlignCenter);

		gridLayout->addWidget(notStartedLabel, 0, 0, 1, 1);

		addWidget(notStarted);

		addWidget(QWidget::createWindowContainer(app->window()));
		connect(app->window(), &Model::OgreWindow::clicked, this, &ApplicationWindow::clicked);

		addWidget(new ApplicationLog(mApp));

		setCurrentIndex(0);
	

	}

	

	void ApplicationWindow::toggleLog()
	{
		if (currentIndex() == 2) {
			setCurrentIndex(mApp->isRunning() ? 1 : 0);
		}
		else {
			setCurrentIndex(2);
		}	
	}

} // namespace View
} // namespace Maditor
