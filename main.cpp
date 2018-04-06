#include "view/maditorviewlib.h"

#include "view/maditorview.h"

#include "model/maditor.h"

#include <QApplication>

int main(int argc, char **argv) {

	QApplication app(argc, argv);

	qRegisterMetaType<Engine::ValueType>();

	Maditor::Model::Maditor model;

	Maditor::View::MaditorView view;
	view.setModel(&model);

	view.createMainWindow();

	return app.exec();
}
