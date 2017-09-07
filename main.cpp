#include "View/maditorviewlib.h"

#include "View/mainwindow.h"

#include "Model/maditor.h"

#include <QApplication>

int main(int argc, char **argv) {

	QApplication app(argc, argv);

	qRegisterMetaType<Engine::ValueType>();

	Maditor::Model::Maditor model;

	Maditor::View::MainWindow window(&model);
	window.show();

	return app.exec();
}