#include "maditorviewlib.h"


#include "applicationwindow.h"

#include "Model\Application\ApplicationLauncher.h"

#include "Model\Application\OgreWindow.h"


namespace Maditor {
namespace View {

	ApplicationWindow::ApplicationWindow(QWidget *parent) :
		QStackedWidget(parent){

	}


	void ApplicationWindow::setModel(Model::ApplicationLauncher * app)
	{
		addWidget(QWidget::createWindowContainer(app->window()));

		setCurrentIndex(1);
	}



} // namespace View
} // namespace Maditor
