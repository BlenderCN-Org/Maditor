#pragma once

#if Maditor_View_EXPORTS
#define MADITOR_VIEW_EXPORT __declspec(dllexport)
#else
#define MADITOR_VIEW_EXPORT __declspec(dllimport)
#endif

#include "Model\maditormodellib.h"

#include "maditorforward.h"

#include <QDialog>

#include <QMessageBox>

#include <QFileDialog>

#include <QPushButton>

#include <QStackedWidget>

#include <QCheckBox>

#include <QCloseEvent>

