#pragma once

#include "ComponentView.h"

#include "Model\Logs\LogTableModel.h"

namespace Maditor {
namespace View {



class LogsView : public QObject, public ComponentView<Model::LogTableModel>
{
    Q_OBJECT

public:

	LogsView();
	
	void setupUi(Ui::MainWindow *ui, QMainWindow *window);
	virtual void setModel(Model::LogTableModel *logs) override;


private:
	Ui::MainWindow *mUi;
};


} // namespace View
} // namespace Maditor

