#ifndef MADITOR_VIEW_MAINWINDOW_H
#define MADITOR_VIEW_MAINWINDOW_H

#include <QMainWindow>

namespace Maditor {
namespace View {

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Model::Maditor *model);
    ~MainWindow();

private slots:
	void onProjectOpened(Model::Project *project);

private:
    Ui::MainWindow *ui;

	Dialogs::DialogManager *mDialogManager;
};


} // namespace View
} // namespace Maditor
#endif // MADITOR_VIEW_MAINWINDOW_H
