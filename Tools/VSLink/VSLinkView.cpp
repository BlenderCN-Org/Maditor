#include <maditorviewlib.h>

#include "VSLinkView.h"

#include "VSLink.h"

#include "vssettingswidget.h"

#include "View/mainwindow.h"

VSLinkView::VSLinkView(VSLink *model) {
	setModel(model);
}

void VSLinkView::setupUi(Maditor::View::Ui::MainWindow *ui, Maditor::View::MainWindow * window)
{
	QAction *openAction = new QAction("OpenSolution");

	createToolbar(window, "VSLink", {
		openAction
	});

	setConnections({
		{openAction, &VSLink::openSolution}
	});

	createSettingsTab(window->dialogs(), new VSSettingsWidget(model()), "Visual Studio");
	
}


