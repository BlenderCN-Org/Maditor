#pragma once

#include "ComponentView.h"

#include "Model\Application\ApplicationLauncher.h"

namespace Maditor {
namespace View {



class ApplicationView : public QObject, public ComponentView<Model::ApplicationLauncher>
{
    Q_OBJECT

public:

	ApplicationView();
	
	void setupUi(Ui::MainWindow *ui, MainWindow *window);
	virtual void setModel(Model::ApplicationLauncher *app) override;

private slots:
	void onApplicationInitializing();
	void onApplicationInitialized();
	void onApplicationStarted();
	void onApplicationStopped();
	void onApplicationShutdown();
	

private:
	Ui::MainWindow *mUi;
};


} // namespace View
} // namespace Maditor

