#pragma once

/// @cond


#include "maditormodelforward.h"
#include "Shared/maditorsharedlib.h"

#if defined(Maditor_Model_EXPORTS)
#define MADITOR_MODEL_EXPORT DLL_EXPORT
#else
#define MADITOR_MODEL_EXPORT DLL_IMPORT
#endif


#include "Madgine/baselib.h"

//before any qt because of emit
#include "Madgine/SignalSlot\signal.h"
#include "Madgine/Serialize\Container\serialized.h"
#include "Madgine/Serialize\Container\observed.h"
#include "Madgine/Serialize\Container\observablecontainer.h"

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
#include <QAbstractListModel>
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
#include <QTimer>

Q_DECLARE_METATYPE(Engine::ValueType);

/// @endcond