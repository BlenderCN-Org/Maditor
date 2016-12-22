#ifndef MADITOR_VIEW_MAINWINDOW_H
#define MADITOR_VIEW_MAINWINDOW_H

#include <QMainWindow>

namespace Maditor {
namespace View {

namespace Ui {
class MainWindow;
}

class MADITOR_VIEW_EXPORT MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Model::Maditor *model);
    ~MainWindow();

	virtual void closeEvent(QCloseEvent * event) override;

	Dialogs::DialogManager *dialogs();

public slots:
	void clearRecentProjects();

private slots:
	void onProjectOpened(Model::Project *project);

	void updateRecentProjects(const QStringList &list);
	void recentProjectClicked(QAction *action);


private:
    Ui::MainWindow *ui;

	Dialogs::DialogManager *mDialogManager;

	Model::Maditor *mModel;

	int mRecentProjectInitialActionCount;

	EditorSettingsWidget *mEditorSettingsWidget;

};


} // namespace View
} // namespace Maditor
#endif // MADITOR_VIEW_MAINWINDOW_H
