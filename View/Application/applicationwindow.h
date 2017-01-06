#pragma once

namespace Maditor {
namespace View {



class ApplicationWindow : public QStackedWidget
{
    Q_OBJECT

public:
	ApplicationWindow(QWidget *parent = nullptr);

	void setModel(Model::ApplicationLauncher *app);


};


} // namespace View
} // namespace Maditor

