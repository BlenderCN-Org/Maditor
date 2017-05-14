#pragma once

#include "Documents\DocumentView.h"

namespace Maditor {
namespace View {



class ApplicationWindow : public QStackedWidget, public DocumentView
{
    Q_OBJECT

public:
	ApplicationWindow(Model::ApplicationLauncher *app);

	Model::ApplicationLauncher *app();

private slots:
	void onApplicationSettingup();
	void onApplicationSetup();
	void onApplicationStarted();
	void onApplicationStopped();
	void onApplicationShutdown();

	void toggleLog();

private:
	Model::ApplicationLauncher *mApp;

};


} // namespace View
} // namespace Maditor

