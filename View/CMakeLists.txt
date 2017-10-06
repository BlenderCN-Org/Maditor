PROJECT(Maditor_View)

set (CMAKE_AUTOUIC ON)
set (CMAKE_AUTORCC ON)


# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

set (CMAKE_AUTOMOC_MOC_OPTIONS "-bmaditorviewlib.h")


set (sources "")
FolderSources(sources "" "" componentview.h editorsettingswidget.cpp editorsettingswidget.h editorsettingswidget.ui maditorview.cpp maditorview.h maditorviewforward.h maditorviewlib.cpp maditorviewlib.h mainwindow.cpp mainwindow.h mainwindow.ui resources.qrc TreeView.cpp TreeView.h)
	FolderSources(sources "" Application applicationlog.cpp applicationlog.h applicationlog.ui ApplicationView.cpp ApplicationView.h ApplicationWindow.cpp ApplicationWindow.h appstatswidget.cpp appstatswidget.h appstatswidget.ui configwidget.cpp configwidget.h configwidget.ui)
		FolderSources(sources Application Inspector inspectorview.cpp inspectorview.h inspectorview.ui valuetypeedit.cpp valuetypeedit.h)
	FolderSources(sources "" Dialogs DialogManager.cpp DialogManager.h NewClassDialog.cpp NewClassDialog.h NewClassDialog.ui NewGuiHandlerDialog.cpp NewGuiHandlerDialog.h NewGuiHandlerDialog.ui NewModuleDialog.cpp NewModuleDialog.h NewModuleDialog.ui NewOtherClassDialog.cpp NewOtherClassDialog.h NewOtherClassDialog.ui NewProjectDialog.cpp NewProjectDialog.h NewProjectDialog.ui settingsdialog.cpp settingsdialog.h settingsdialog.ui settingstab.h)
	FolderSources(sources "" Documents CentralWidget.cpp CentralWidget.h DocumentTabBar.cpp DocumentTabBar.h DocumentTabWidget.cpp DocumentTabWidget.h DocumentView.cpp DocumentView.h IndependentWindowSpawner.h WindowSpawner.h)
	FolderSources(sources "" Logs LogsView.cpp LogsView.h LogWidget.cpp LogWidget.h)
	FolderSources(sources "" Project FilesView.cpp FilesView.h ModulePropertiesWidget.cpp ModulePropertiesWidget.h ModulePropertiesWidget.ui ProjectView.cpp ProjectView.h propertiesWidget.cpp propertiesWidget.h propertiesWidget.ui)

add_library(Maditor_View SHARED ${sources})

include_directories(${CMAKE_SOURCE_DIR})
add_precompiled_header(Maditor_View maditorviewlib.h FORCEINCLUDE SOURCE_CXX maditorviewlib.cpp)

include_targets(Maditor_View Maditor_Model)



