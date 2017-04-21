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

	setInputEnabled(app->isRunning());

	connect(app, &Model::ApplicationLauncher::outputReceived, this, &ApplicationLog::output);
	connect(app, &Model::ApplicationLauncher::applicationSetup, this, &ApplicationLog::enableInput);
	connect(app, &Model::ApplicationLauncher::applicationShutdown, this, &ApplicationLog::disableInput);
	connect(ui->clearButton, &QPushButton::clicked, this, &ApplicationLog::clear);
	connect(ui->sendButton, &QPushButton::clicked, this, &ApplicationLog::sendLine);
	connect(ui->stdinEdit, &QLineEdit::returnPressed, this, &ApplicationLog::sendLine);
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

	void ApplicationLog::sendLine()
	{
		if (mApp->isLaunched())
			mApp->sendCommand(ui->stdinEdit->text());
		ui->stdinEdit->clear();
	}

	void ApplicationLog::enableInput()
	{
		setInputEnabled(true);
	}

	void ApplicationLog::disableInput()
	{
		setInputEnabled(false);
	}

	void ApplicationLog::setInputEnabled(bool b)
	{
		ui->stdinEdit->setEnabled(b);
		ui->sendButton->setEnabled(b);
	}

	void ApplicationLog::output(const QString & msg)
	{
		ui->log->moveCursor(QTextCursor::End);
		ui->log->insertPlainText(msg);
		ui->log->moveCursor(QTextCursor::End);
	}


} // namespace View
} // namespace Maditor
