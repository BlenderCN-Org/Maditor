#pragma once

#include "ComponentView.h"

#include "Model\Project\Project.h"

#include "Documents\IndependentWindowSpawner.h"
#include "Documents\WindowSpawner.h"

#include "propertieswidget.h"
#include "Application\configwidget.h"

namespace Maditor {
namespace View {



class ProjectView : 
	public QObject, 
	public ComponentView<Model::Project>, 
	public IndependentWindowSpawner<Model::ApplicationConfig, ConfigWidget>,
	public IndependentWindowSpawner<Model::Project, PropertiesWidget>
{
    Q_OBJECT

public:

	ProjectView();
	
	void setupUi(Ui::MainWindow *ui, QMainWindow *window);
	virtual void setModel(Model::Project *project) override;

private slots:
	void openConfig(Model::ApplicationConfig *config);

private:
	Ui::MainWindow *mUi;
};


} // namespace View
} // namespace Maditor

