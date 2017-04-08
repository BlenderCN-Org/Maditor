#include "maditorviewlib.h"

#include "applicationlog.h"
#include "ui_applicationlog.h"

#include "Model\Application\ApplicationLauncher.h"

namespace Maditor {
namespace View {

	ApplicationLog::ApplicationLog(Model::ApplicationLauncher *config) :
	DocumentView(config, this),
    ui(new Ui::ApplicationLog),
	mConfig(config)
{
    ui->setupUi(this);

	
	/*ui->launcherGroup->setId(ui->maditorLauncherButton, Model::ApplicationConfig::MADITOR_LAUNCHER);
	ui->launcherGroup->setId(ui->customLauncherButton, Model::ApplicationConfig::CUSTOM_LAUNCHER);

	ui->launcherTypeGroup->setId(ui->clientButton, Model::ApplicationConfig::CLIENT_LAUNCHER);
	ui->launcherTypeGroup->setId(ui->serverButton, Model::ApplicationConfig::SERVER_LAUNCHER);

	ui->launcherGroup->button(config->launcher())->setChecked(true);
	ui->launcherTypeGroup->button(config->launcherType())->setChecked(true);

	connect(ui->launcherGroup, static_cast<void(QButtonGroup::*)(int, bool)>(&QButtonGroup::buttonToggled), [=](int id, bool checked) {if (checked) mConfig->setLauncher((Model::ApplicationConfig::Launcher)id); });
	connect(ui->launcherTypeGroup, static_cast<void(QButtonGroup::*)(int, bool)>(&QButtonGroup::buttonToggled), [=](int id, bool checked) {if (checked) mConfig->setLauncherType((Model::ApplicationConfig::LauncherType)id); });
	connect(ui->customLauncherCmd, &QLineEdit::textChanged, config, &Model::ApplicationConfig::setCustomExecutableCmd);*/
}

	ApplicationLog::~ApplicationLog()
{
    delete ui;
}


} // namespace View
} // namespace Maditor
