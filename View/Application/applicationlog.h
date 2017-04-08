#pragma once

#include "Documents\DocumentView.h"

namespace Maditor {
namespace View {

namespace Ui {
class ApplicationLog;
}

class ApplicationLog : public QWidget, public DocumentView
{
    Q_OBJECT

public:
    explicit ApplicationLog(Model::ApplicationLauncher *config);
    ~ApplicationLog();

private:
    Ui::ApplicationLog *ui;

	Model::ApplicationLauncher *mConfig;

};


} // namespace View
} // namespace Maditor

