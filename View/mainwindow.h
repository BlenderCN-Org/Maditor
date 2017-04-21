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


private:
    Ui::MainWindow *ui;

	MaditorView *mMaditor;

	QSettings &mSettings;

};


} // namespace View
} // namespace Maditor
#endif // MADITOR_VIEW_MAINWINDOW_H
