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
FolderSources(sources "" "" componentview.h editorsettingswidget.cpp editorsettingswidget.h editorsettingswidget.ui maditorviewforward.h maditorviewlib.cpp maditorviewlib.h mainwindow.cpp mainwindow.h mainwindow.ui resources.qrc TreeView.cpp TreeView.h WindowSpawner.h)
	FolderSources(sources "" Application ApplicationView.cpp ApplicationView.h ApplicationWindow.cpp ApplicationWindow.h appstatswidget.cpp appstatswidget.h appstatswidget.ui)
	FolderSources(sources "" Dialogs DialogManager.cpp DialogManager.h NewClassDialog.cpp NewClassDialog.h NewClassDialog.ui NewGuiHandlerDialog.cpp NewGuiHandlerDialog.h NewGuiHandlerDialog.ui NewModuleDialog.cpp NewModuleDialog.h NewModuleDialog.ui NewProjectDialog.cpp NewProjectDialog.h NewProjectDialog.ui settingsdialog.cpp settingsdialog.h settingsdialog.ui settingstab.h)
	FolderSources(sources "" Documents DocumentView.cpp DocumentView.h)
	FolderSources(sources "" Logs LogsTabWidget.cpp LogsTabWidget.h LogsView.cpp LogsView.h LogWidget.cpp LogWidget.h)
	FolderSources(sources "" Project FilesView.cpp FilesView.h ModuleListPropertiesWidget.cpp ModuleListPropertiesWidget.h ModuleListPropertiesWidget.ui ModulePropertiesWidget.cpp ModulePropertiesWidget.h ModulePropertiesWidget.ui ProjectView.cpp ProjectView.h)

add_library(Maditor_View SHARED ${sources})

add_precompiled_header(Maditor_View maditorviewlib.h FORCEINCLUDE SOURCE_CXX maditorviewlib.cpp)

target_link_libraries(Maditor_View Maditor_Model)

target_link_libraries(Maditor_View Qt5::Widgets)


