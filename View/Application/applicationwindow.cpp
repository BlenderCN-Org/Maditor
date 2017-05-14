#include "maditorviewlib.h"


#include "applicationwindow.h"

#include "Model\Application\ApplicationLauncher.h"

#include "Model\Application\OgreWindow.h"

#include "applicationlog.h"


namespace Maditor {
namespace View {

	ApplicationWindow::ApplicationWindow(Model::ApplicationLauncher *app) :
		QStackedWidget(nullptr),
		DocumentView(app, this),
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
		notStartedLabel->setObjectName(QStringLiteral("notStartedLabel"));
		notStartedLabel->setAlignment(Qt::AlignCenter);

		gridLayout->addWidget(notStartedLabel, 0, 0, 1, 1);

		addWidget(notStarted);

		addWidget(QWidget::createWindowContainer(app->window()));

		addWidget(new ApplicationLog(mApp));

		setCurrentIndex(0);

		connect(app, &Model::ApplicationLauncher::applicationSettingup, this, &ApplicationWindow::onApplicationSettingup);
		connect(app, &Model::ApplicationLauncher::applicationSetup, this, &ApplicationWindow::onApplicationSetup);
		connect(app, &Model::ApplicationLauncher::applicationStarted, this, &ApplicationWindow::onApplicationStarted);
		connect(app, &Model::ApplicationLauncher::applicationStopped, this, &ApplicationWindow::onApplicationStopped);
		connect(app, &Model::ApplicationLauncher::applicationShutdown, this, &ApplicationWindow::onApplicationShutdown);

		setContextMenuPolicy(Qt::ActionsContextMenu);
		QAction *toggleAction = new QAction("Toggle Log");
		addAction(toggleAction);
		connect(toggleAction, &QAction::triggered, this, &ApplicationWindow::toggleLog);
	}


	void ApplicationWindow::onApplicationSettingup()
	{
		if (currentIndex() != 2)
			setCurrentIndex(1);
	}

	Model::ApplicationLauncher * ApplicationWindow::app()
	{
		return mApp;
	}

	void ApplicationWindow::onApplicationSetup()
	{
		if (currentIndex() != 2)
			setCurrentIndex(0);
	}

	void ApplicationWindow::onApplicationStarted()
	{
		if (currentIndex() != 2)
			setCurrentIndex(1);
	}

	void ApplicationWindow::onApplicationStopped()
	{
		if (currentIndex() != 2)
			setCurrentIndex(0);
	}

	void ApplicationWindow::onApplicationShutdown() 
	{
		if (currentIndex() != 2)
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
