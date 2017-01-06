#pragma once


#include "View\ComponentView.h"

#include "VSLink.h"

class VSSettingsWidget;

class VSLinkView : public QObject, public Maditor::View::ComponentView<VSLink> {

	Q_OBJECT

public:
	VSLinkView(VSLink *model);
	
	void setupUi(Maditor::View::Ui::MainWindow *ui, Maditor::View::MainWindow *window);

private:

	VSSettingsWidget *mSettingsWidget;
};
