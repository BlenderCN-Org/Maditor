#include "maditorviewlib.h"

#include "applicationlog.h"
#include "ui_applicationlog.h"

#include "Model\Application\ApplicationLauncher.h"

namespace Maditor {
namespace View {

	ApplicationLog::ApplicationLog(Model::ApplicationLauncher *app) :
	DocumentView(app, this),
    ui(new Ui::ApplicationLog),
	mApp(app)
{
    ui->setupUi(this);

	connect(app, &Model::ApplicationLauncher::outputReceived, this, &ApplicationLog::output);
	connect(ui->clearButton, &QPushButton::clicked, this, &ApplicationLog::clear);
}

	ApplicationLog::~ApplicationLog()
{
    delete ui;
}

	Model::ApplicationLauncher * ApplicationLog::app()
	{
		return mApp;
	}

	void ApplicationLog::clear()
	{
		ui->log->clear();
	}

	void ApplicationLog::output(const QString & msg)
	{
		ui->log->moveCursor(QTextCursor::End);
		ui->log->insertPlainText(msg);
		ui->log->moveCursor(QTextCursor::End);
	}


} // namespace View
} // namespace Maditor
