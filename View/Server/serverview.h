#pragma once

#include "ComponentView.h"

#include "Model\Project\Project.h"

namespace Maditor {
namespace View {



class ServerView : public QObject, public ComponentView<Model::Project>
{
    Q_OBJECT

public:

	ServerView();
	
	void setupUi(Ui::MainWindow *ui, MainWindow *window);
	virtual void setModel(Model::Project *project) override;

private slots:
	void addGenerator(Model::Generators::ClassGenerator *gen);
	void createServerWindow(Model::ServerLauncher *launcher);

private:
	Ui::MainWindow *mUi;

	std::map<Model::Generators::ServerClassGenerator *, QAction*> mServerActions;
	std::map<Model::ServerLauncher*, ServerWindow*> mServerWindows;
	QMenu *mStartServerMenu;
};


} // namespace View
} // namespace Maditor

