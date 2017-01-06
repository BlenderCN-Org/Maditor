#pragma once

#if Maditor_Model_EXPORTS
#define MADITOR_MODEL_EXPORT __declspec(dllexport)
#else
#define MADITOR_MODEL_EXPORT __declspec(dllimport)
#endif

#include "maditormodelforward.h"
#include "Shared/maditorsharedlib.h"

#include <memory>

#include <QString>
#include <QWindow>
#include <QBrush>
#include <QPainter>
#include <QObject>
#include <QTextStream>
#include <QMenu>
#include <QDomElement>
#include <QSortFilterProxyModel>
#include <QAbstractItemModel>
#include <QFileSystemModel>
#include <QFileSystemWatcher>
#include <QMessageBox>
#include <QSet>
#include <QDebug>
#include <QApplication>
#include <QStyle>
#include <QSettings>
#include <QKeyEvent>
#include <QTime>