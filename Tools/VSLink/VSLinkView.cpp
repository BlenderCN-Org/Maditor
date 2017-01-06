#include <maditorviewlib.h>

#include "VSLinkView.h"

#include "VSLink.h"

#include "vssettingswidget.h"

#include "View\mainwindow.h"

#include "View\Dialogs\DialogManager.h"

#include "View\Dialogs\settingsdialog.h"

VSLinkView::VSLinkView(VSLink *model) {
	setModel(model);
}

void VSLinkView::setupUi(Maditor::View::Ui::MainWindow *ui, Maditor::View::MainWindow * window)
{
	QAction *openAction = new QAction("OpenSolution");

	createToolbar(window, "VSLink_Toolbar", {
		{openAction, &VSLink::openSolution}
	});

	mSettingsWidget = new VSSettingsWidget(model());
	Maditor::View::Dialogs::SettingsDialog *settings = window->dialogs()->settingsDialog();
	settings->addSettingsTab(mSettingsWidget, "Visual Studio");

}


