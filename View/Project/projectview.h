#pragma once

#include "ComponentView.h"

#include "Model\Project\Project.h"

namespace Maditor {
namespace View {



class ProjectView : public QObject, public ComponentView<Model::Project>
{
    Q_OBJECT

public:

	ProjectView();
	
	void setupUi(Ui::MainWindow *ui, QMainWindow *window);
	virtual void setModel(Model::Project *project) override;

private:
	Ui::MainWindow *mUi;
};


} // namespace View
} // namespace Maditor

