#ifndef MADITOR_VIEW_SERVERWINDOW_H
#define MADITOR_VIEW_SERVERWINDOW_H

#include <QWidget>

namespace Maditor {
namespace View {

namespace Ui {
class ServerWindow;
}

class ServerWindow : public QWidget
{
    Q_OBJECT

public:
	explicit ServerWindow(Model::ServerLauncher *model, QTabWidget *tabWidget);
    ~ServerWindow();

private slots:
	void onServerStarting();
	void onServerStarted();
	void onServerShutdown();

private:
    Ui::ServerWindow *ui;

	Model::ServerLauncher *mModel;

	QTabWidget *mTabWidget;
};


} // namespace View
} // namespace Maditor
#endif // MADITOR_VIEW_SERVERWINDOW_H
