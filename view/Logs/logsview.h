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
	
	void setupUi(MainWindow *window);
	virtual void setModel(Model::LogTableModel *logs) override;

private:
	LogWidget *mLogsWidget;

};


} // namespace View
} // namespace Maditor

