#pragma once

#include "ComponentView.h"

#include "Model\Logs\LogsModel.h"

namespace Maditor {
namespace View {



class LogsView : public QObject, public ComponentView<Model::LogsModel>
{
    Q_OBJECT

public:

	LogsView();
	
	void setupUi(Ui::MainWindow *ui, QMainWindow *window);
	virtual void setModel(Model::LogsModel *logs) override;


private:
	Ui::MainWindow *mUi;
};


} // namespace View
} // namespace Maditor

