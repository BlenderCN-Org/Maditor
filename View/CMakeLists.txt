PROJECT(Maditor_View)

set (CMAKE_AUTOUIC ON)
set (CMAKE_AUTORCC ON)


# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

set (CMAKE_AUTOMOC_MOC_OPTIONS "-bmaditorviewlib.h")

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${MADGINE_INCLUDE_DIRS})


set (sources "")
FolderSources(sources "" "" applicationwindow.cpp applicationwindow.h editorsettingswidget.cpp editorsettingswidget.h editorsettingswidget.ui maditorviewlib.cpp maditorviewlib.h mainwindow.cpp mainwindow.h mainwindow.ui resources.qrc)
FolderSources(sources "" Dialogs DialogManager.cpp DialogManager.h ModulePropertiesDialog.cpp ModulePropertiesDialog.h ModulePropertiesDialog.ui NewClassDialog.cpp NewClassDialog.h NewClassDialog.ui NewGuiHandlerDialog.cpp NewGuiHandlerDialog.h NewGuiHandlerDialog.ui NewModuleDialog.cpp NewModuleDialog.h NewModuleDialog.ui settingsdialog.cpp settingsdialog.h settingsdialog.ui settingstab.h)

add_library(Maditor_View SHARED ${sources})

add_precompiled_header(Maditor_View maditorviewlib.h FORCEINCLUDE SOURCE_CXX maditorviewlib.cpp)

target_link_libraries(Maditor_View Maditor_Model)

target_link_libraries(Maditor_View Qt5::Widgets)


