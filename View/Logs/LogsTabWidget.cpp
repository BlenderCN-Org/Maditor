#include "maditorviewlib.h"

#include "LogsTabWidget.h"
#include "Model\Logs\LogsModel.h"
#include "Model\Logs\log.h"
#include "logwidget.h"

namespace Maditor {
	namespace View {

		LogsTabWidget::LogsTabWidget(QWidget *parent) :
			QTabWidget(parent){}
		

		void LogsTabWidget::setModel(const Model::LogsModel * model)
		{
			connect(model, &Model::LogsModel::logWatcherCreated, this, &LogsTabWidget::addLog);
			connect(model, &Model::LogsModel::logWatcherDeleted, this, &LogsTabWidget::removeLog);
			for (Model::Log *log : model->logs())
				addLog(log);
		}

		void LogsTabWidget::removeLog(Model::Log * log)
		{
			auto it = mTabs.find(log);
			QWidget *w = it.value();
			removeTab(indexOf(w));
			//w->deleteLater();
			mTabs.erase(it);
		}

		void LogsTabWidget::addLog(Model::Log *log) {
			if (log->type() == Model::Log::TextLog) {
				QPlainTextEdit *edit = new QPlainTextEdit;
				mTabs[log] = edit;
				addTab(edit, QString::fromStdString(log->name()));
				connect(log, &Model::Log::messageReceived, edit, &QPlainTextEdit::appendPlainText);
			}else{
				LogWidget *view = new LogWidget;
				mTabs[log] = view;
				view->setModel(log->model());
				addTab(view, QString::fromStdString(log->name()));
			}
			
		}

	}

}

