#pragma once

#include <QMainWindow>

namespace Maditor {
namespace View {

	namespace Ui {
		class MainWindow;
	}

class ApplicationWindow : public QStackedWidget
{
    Q_OBJECT

public:
	ApplicationWindow(QWidget *parent = nullptr);

	void setupUi(Ui::MainWindow *ui, QMainWindow *window);
	void setModel(Model::ApplicationLauncher *app);

private slots:
	void onApplicationInitializing();
	void onApplicationShutdown();

private:
	Ui::MainWindow *mUi;
};


} // namespace View
} // namespace Maditor

