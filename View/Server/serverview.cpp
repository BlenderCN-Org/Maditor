#include "maditorviewlib.h"

#include "mainwindow.h"
#include "mainwindow_ui.h"

#include "serverview.h"

#include "Model/Project/ModuleList.h"

#include "Model/Project/Generators/ServerClassGenerator.h"

#include "serverwindow.h"

namespace Maditor {
namespace View {

	ServerView::ServerView() :
		mUi(0),
		mStartServerMenu(0)
	{

	}

	void ServerView::setupUi(Ui::MainWindow * ui, MainWindow * window)
	{
		mUi = ui;

		/*setConnections({
			{ ui->actionInit, &Model::ApplicationLauncher::init },
			{ ui->actionInit_No_Debug, &Model::ApplicationLauncher::initNoDebug },
			{ ui->actionShutdown, &Model::ApplicationLauncher::shutdown },
			{ ui->actionKill, &Model::ApplicationLauncher::kill},
			{ ui->actionStart, &Model::ApplicationLauncher::start },
			{ ui->actionPause, &Model::ApplicationLauncher::pause },
			{ ui->actionStop, &Model::ApplicationLauncher::stop },
		});*/

		mStartServerMenu = new QMenu("Start Server");
		mStartServerMenu->setEnabled(false);

		setConnections({
			{ mStartServerMenu->menuAction(), &Model::Project::startDefaultServer}
		});

		createToolbar(window, "ServerToolbar", {
			mStartServerMenu->menuAction()
		});


	}

	void ServerView::setModel(Model::Project * project)
	{
		ComponentView::setModel(project);

		connect(project->moduleList(), &Model::ModuleList::classAdded, this, &ServerView::addGenerator);
		connect(project, &Model::Project::serverCreated, this, &ServerView::createServerWindow);

		for (const std::unique_ptr<Model::Module> &module : *project->moduleList()) {
			for (const std::unique_ptr<Model::Generators::ClassGenerator> &generator : module->getClasses()) {
				addGenerator(generator.get());
			}
		}

	}

	void ServerView::createServerWindow(Model::ServerLauncher * launcher)
	{
		ServerWindow *win = new ServerWindow(launcher, mUi->tabWidget);
		mServerWindows[launcher] = win;
		mUi->tabWidget->addTab(win, launcher->name());
	}

	void ServerView::addGenerator(Model::Generators::ClassGenerator *gen) {
		if (gen->type() == "Server") {
			Model::Generators::ServerClassGenerator *server = static_cast<Model::Generators::ServerClassGenerator*>(gen);
			QAction *action = mStartServerMenu->addAction(gen->fullName(), [=]() { model()->startServer(server); });
			mServerActions[server] = action;
			mStartServerMenu->setEnabled(true);
		}
	}


} // namespace View
} // namespace Maditor
