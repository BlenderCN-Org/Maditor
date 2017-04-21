#include "maditorviewlib.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "Model\maditor.h"

#include "maditorview.h"


#include "Model\Addons\Addon.h"

namespace Maditor {
namespace View {

	MainWindow::MainWindow(Model::Maditor *model) :
		ui(new Ui::MainWindow),
		mSettings(model->settings()),
		mMaditor(new MaditorView)
	{
		ui->setupUi(this);

		mMaditor->setupUi(ui, this);
		model->addons()->setupUi(ui, this);		

		QMenu *menu = createPopupMenu();
		menu->setTitle("Views");
		menuBar()->addMenu(menu);

		show();

		mSettings.beginGroup("Window");
		restoreGeometry(mSettings.value("geometry").toByteArray());
		restoreState(mSettings.value("state").toByteArray(), 0);
		mSettings.endGroup();

		connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::close);

		mMaditor->setModel(model);
	}

MainWindow::~MainWindow()
{
    delete ui;
	delete mMaditor;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	if (!mMaditor->closeEvent()) {
		event->ignore();
		return;
	}

	mSettings.beginGroup("Window");
	mSettings.setValue("geometry", saveGeometry());
	mSettings.setValue("state", saveState(0));
	mSettings.endGroup();

	event->accept();
}

Dialogs::DialogManager * MainWindow::dialogs()
{
	return mMaditor->dialogs();
}





} // namespace View
} // namespace Maditor
