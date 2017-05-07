cmake_minimum_required (VERSION 2.6)

cmake_policy(SET CMP0011 OLD)
cmake_policy(SET CMP0049 OLD)


PROJECT(Maditor_Launcher)


set (sources "")
FolderSources(sources "" "" ApplicationWrapper.cpp ApplicationWrapper.h InputWrapper.cpp InputWrapper.h maditorlauncherlib.cpp maditorlauncherlib.h maditorlauncherforward.h main.cpp ModuleLoader.cpp ModuleLoader.h)
	FolderSources(sources "" Util AppStats.cpp AppStats.h LogListener.cpp LogListener.h Util.cpp Util.h)


include_directories(${CMAKE_CURRENT_SOURCE_DIR})


add_executable(Maditor_Launcher ${sources})

add_precompiled_header(Maditor_Launcher ${CMAKE_CURRENT_SOURCE_DIR}/maditorlauncherlib.h FORCEINCLUDE SOURCE_CXX maditorlauncherlib.cpp)

include_targets(Maditor_Launcher Maditor_Shared)
include_library(Maditor_Launcher Madgine OgreMadgine MadgineServer)
