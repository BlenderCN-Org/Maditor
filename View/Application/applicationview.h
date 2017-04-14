#pragma once

#include "ComponentView.h"
#include "Documents\WindowSpawner.h"

#include "Model\Application\ApplicationLauncher.h"

#include "applicationwindow.h"
#include "applicationlog.h"

namespace Maditor {
namespace View {



class ApplicationView : 
	public QObject, 
	public ComponentView<Model::ApplicationLauncher>, 
	public WindowSpawner<Model::ApplicationLauncher, ApplicationWindow>, 
	public WindowSpawner<Model::ApplicationLauncher, ApplicationLog>
{
    Q_OBJECT

public:

	ApplicationView();
	
	void setupUi(Ui::MainWindow *ui, MainWindow *window);

	void setConfigModel(Model::ConfigList *list);

protected:
	virtual void setModel(Model::ApplicationLauncher *app) override;
	virtual void clearModel() override;

	virtual void currentTabSet(ApplicationWindow *win) override;
	virtual void currentTabSet(ApplicationLog *win) override;
	virtual void currentTabCleared(QWidget *w) override;

	void selectConfig(QAction *action);
	void selectConfigName(const QString &name);

private slots:
	void onConfigAdded(Model::ApplicationConfig *config);
	void onInstanceAdded(Model::ApplicationLauncher *instance);
	void onInstanceDestroyed(Model::ApplicationLauncher *instance);
	void createCurrentConfig();

	void onApplicationSettingup();
	void onApplicationSetup();
	void onApplicationStarted();
	void onApplicationStopped();
	void onApplicationShutdown();
	

private:
	Ui::MainWindow *mUi;

	Model::ConfigList *mList;

	QMenu mCurrentConfigSelector;

	QWidget *mCurrentWidget;

	std::list<QMetaObject::Connection> mAppConnections;
};


} // namespace View
} // namespace Maditor

