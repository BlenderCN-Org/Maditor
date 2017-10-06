#include "View/maditorviewlib.h"

#include "View/maditorview.h"

#include "Model/maditor.h"

#include <QApplication>

int main(int argc, char **argv) {

	QApplication app(argc, argv);

	qRegisterMetaType<Engine::ValueType>();

	Maditor::Model::Maditor model;

	Maditor::View::MaditorView view;
	view.setModel(&model);

	view.createMainWindow();

	/*Maditor::View::MainWindow window(&model);
	window.show();*/

	return app.exec();
}