#pragma once

#include "ComponentView.h"

#include "Model\maditor.h"


namespace Maditor {
namespace View {



class MaditorView : 
	public QObject, 
	public ComponentView<Model::Maditor> 
{
    Q_OBJECT

public:

	MaditorView();
	virtual ~MaditorView();
	
	void setupUi(Ui::MainWindow *ui, MainWindow *window);
	virtual void setModel(Model::Maditor *model) override;

	Dialogs::DialogManager *dialogs();

	bool closeEvent();

private slots:
	void onProjectOpened(Model::Project *project);

	void updateRecentProjects(const QStringList &list);
	void recentProjectClicked(QAction *action);

private:
	Ui::MainWindow *mUi;

	Dialogs::DialogManager *mDialogManager;

	int mRecentProjectInitialActionCount;

	LogsView *mLogs;
	ApplicationView *mApplication;
	ProjectView *mProject;

};


} // namespace View
} // namespace Maditor

