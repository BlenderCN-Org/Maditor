PROJECT(Maditor)

set (CMAKE_AUTOUIC ON)
set (CMAKE_AUTORCC ON)


# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

set (CMAKE_AUTOMOC_MOC_OPTIONS "-bmaditorlib.h")

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${MADGINE_INCLUDE_DIRS})


set (sources "")
FolderSources(sources "" "" applicationwindow.cpp applicationwindow.h editorsettingswidget.cpp editorsettingswidget.h editorsettingswidget.ui maditorlib.cpp maditorlib.h main.cpp mainwindow.cpp mainwindow.h mainwindow.ui)
FolderSources(sources "" Dialogs DialogManager.cpp DialogManager.h ModulePropertiesDialog.cpp ModulePropertiesDialog.h ModulePropertiesDialog.ui NewClassDialog.cpp NewClassDialog.h NewClassDialog.ui NewGuiHandlerDialog.cpp NewGuiHandlerDialog.h NewGuiHandlerDialog.ui NewModuleDialog.cpp NewModuleDialog.h NewModuleDialog.ui settingsdialog.cpp settingsdialog.h settingsdialog.ui settingstab.h)

add_executable(Maditor ${sources})

add_precompiled_header(Maditor maditorlib.h FORCEINCLUDE SOURCE_CXX maditorlib.cpp)

target_link_libraries(Maditor Maditor_Model ${TOOLS_LIBS})

target_link_libraries(Maditor Qt5::Widgets)


add_dependencies(Maditor Maditor_Launcher)