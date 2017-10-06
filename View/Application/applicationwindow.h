#pragma once

#include "Documents\DocumentView.h"

#include "ComponentView.h"

namespace Maditor {
namespace View {


	class ApplicationContainerWindow : public QMainWindow, public DocumentView, public ComponentView<Model::ApplicationLauncher> {
		Q_OBJECT

	public:
		ApplicationContainerWindow(Model::ApplicationLauncher *app);

		Model::ApplicationLauncher *app();

	protected:
		virtual void clearModel() override;

	private slots:
		void onApplicationSettingup();
		void onApplicationSetup();
		void onApplicationStarted();
		void onApplicationStopped();
		void onApplicationShutdown();

		void clicked();

	private:
		QAction *actionSetup;
		QAction *actionStart;
		QAction *actionPause;
		QAction *actionStop;
		QAction *actionSetup_No_Debug;
		QAction *actionShutdown;
		QAction *actionKill;


		Model::ApplicationLauncher *mApp;
		ApplicationWindow *mAppWindow;
	};


class ApplicationWindow : public QStackedWidget
{
    Q_OBJECT

public:
	ApplicationWindow(Model::ApplicationLauncher *app);

public slots:
	void toggleLog();

signals:
	void clicked();

private:
	Model::ApplicationLauncher *mApp;

};


} // namespace View
} // namespace Maditor

