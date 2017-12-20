PROJECT(Maditor_Model)



# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

set (CMAKE_AUTOMOC_MOC_OPTIONS "-bmaditormodellib.h")


add_definitions(-DCMAKE_PATH="${CMAKE_COMMAND}")
add_definitions(-DCMAKE_GENERATOR="${CMAKE_GENERATOR}")
add_definitions(-DQSCINTILLA_DLL)


set (sources "")
FolderSources(sources "" "" DialogManager.h maditormodelforward.h maditormodellib.cpp maditormodellib.h maditor.cpp maditor.h MaditorLog.cpp MaditorLog.h TableModel.cpp TableModel.h TreeItem.cpp TreeItem.h TreeModel.cpp TreeModel.h TreeSorter.cpp TreeSorter.h)
	FolderSources(sources "" Addons Addon.cpp Addon.h)
	FolderSources(sources "" Application ApplicationLauncher.cpp ApplicationLauncher.h EmbeddedLauncher.cpp EmbeddedLauncher.h InputWrapper.cpp InputWrapper.h LogReceiver.cpp LogReceiver.h ModuleLoader.cpp ModuleLoader.h OgreWindow.cpp OgreWindow.h ProfilerModel.cpp ProfilerModel.h StandaloneLauncher.cpp StandaloneLauncher.h StatsModel.cpp StatsModel.h TableUnit.cpp TableUnit.h TreeUnit.cpp TreeUnit.h UtilModel.cpp UtilModel.h)
		FolderSources(sources Application Inspector Inspector.cpp Inspector.h scopewrapper.cpp scopewrapper.h)
	FolderSources(sources "" Documents Document.cpp Document.h DocumentStore.h)
	FolderSources(sources "" Logs LogSorterFilter.cpp LogSorterFilter.h LogTableModel.cpp LogTableModel.h)
	FolderSources(sources "" Project ApplicationConfig.cpp ApplicationConfig.h ConfigList.cpp ConfigList.h Module.cpp Module.h ModuleList.cpp ModuleList.h ModuleSelection.cpp ModuleSelection.h Project.cpp Project.h ProjectElement.cpp ProjectElement.h)
		FolderSources(sources Project Generators ClassGenerator.cpp ClassGenerator.h ClassGeneratorFactory.cpp ClassGeneratorFactory.h CmakeGenerator.cpp CmakeGenerator.h CmakeProject.cpp CmakeProject.h CommandLine.cpp CommandLine.h DotGenerator.cpp DotGenerator.h EntityComponentGenerator.cpp EntityComponentGenerator.h GameHandlerGenerator.cpp GameHandlerGenerator.h Generator.cpp Generator.h GlobalAPIGenerator.cpp GlobalAPIGenerator.h GuiHandlerGenerator.cpp GuiHandlerGenerator.h HeaderGuardGenerator.cpp HeaderGuardGenerator.h OtherClassGenerator.cpp OtherClassGenerator.h SceneComponentGenerator.cpp SceneComponentGenerator.h ServerClassGenerator.cpp ServerClassGenerator.h)


add_library(Maditor_Model SHARED ${sources})

add_precompiled_header(Maditor_Model maditormodellib.h FORCEINCLUDE SOURCE_CXX maditormodellib.cpp)

target_link_libraries(Maditor_Model PUBLIC Qt5::Xml Qt5::Gui Qt5::Widgets ${QSCINTILLA2_LIBRARY})

include_targets(Maditor_Model Maditor_Shared)
include_library(Maditor_Model Madgine PRIVATE Base)


