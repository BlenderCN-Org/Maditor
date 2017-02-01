#include "maditorviewlib.h"

#include "appstatswidget.h"
#include "ui_appstatswidget.h"

#include "Model\Application\StatsModel.h"

namespace Maditor {
namespace View {

AppStatsWidget::AppStatsWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AppStatsWidget)
{
    ui->setupUi(this);
}

AppStatsWidget::~AppStatsWidget()
{
    delete ui;
}

void AppStatsWidget::setModel(Model::StatsModel * model)
{
	connect(model, &Model::StatsModel::averageFPSChanged, this, &AppStatsWidget::setFPS);
	connect(model, &Model::StatsModel::memoryUsageChanged, this, &AppStatsWidget::setMemUsage);
	connect(model, &Model::StatsModel::ogreMemoryUsageChanged, this, &AppStatsWidget::setOgreMem);
	connect(ui->StartTrackAllocationsButton, &QPushButton::clicked, model, &Model::StatsModel::trackAllocations);
	connect(ui->StopTrackAllocationsButton, &QPushButton::clicked, model, &Model::StatsModel::logTrackedAllocations);
}

void AppStatsWidget::setFPS(float fps)
{
	ui->FPSValue->setText(fps == 0.0f ? "-" : QLocale().toString(fps));
}

void AppStatsWidget::setMemUsage(size_t mem)
{
	ui->MemUsageValue->setText(mem == 0 ? "-" : QLocale().toString(mem / 1024));
}

void AppStatsWidget::setOgreMem(size_t mem)
{
	ui->OgreMemoryValue->setText(mem == 0 ? "-" : QLocale().toString(mem / 1024));
}



} // namespace View
} // namespace Maditor
