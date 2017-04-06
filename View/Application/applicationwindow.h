#pragma once

namespace Maditor {
namespace View {



class ApplicationWindow : public QStackedWidget
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

private:
	Model::ApplicationLauncher *mApp;

};


} // namespace View
} // namespace Maditor

