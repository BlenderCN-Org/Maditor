cmake_minimum_required (VERSION 2.6)

cmake_policy(SET CMP0011 OLD)
cmake_policy(SET CMP0049 OLD)


PROJECT(Maditor_Launcher)


set (sources "")
FolderSources(sources "" "" ApplicationWrapper.cpp ApplicationWrapper.h EmbeddedWrapper.cpp EmbeddedWrapper.h InputWrapper.cpp InputWrapper.h Inspector.cpp Inspector.h maditorlauncherlib.cpp maditorlauncherlib.h maditorlauncherforward.h main_embedded.cpp ModuleLoader.cpp ModuleLoader.h)
	FolderSources(sources "" Util AppStats.cpp AppStats.h LauncherLogListener.cpp LauncherLogListener.h Util.cpp Util.h)	

include_directories(${CMAKE_CURRENT_SOURCE_DIR})


add_executable(Embedded_Server_Launcher ${sources})
add_executable(Embedded_Client_Launcher ${sources})



include_targets(Embedded_Server_Launcher Maditor_Shared)
include_library(Embedded_Server_Launcher Madgine PRIVATE MadgineServer)
include_library(Embedded_Server_Launcher Lua PRIVATE lualib)

include_targets(Embedded_Client_Launcher Maditor_Shared)
include_library(Embedded_Client_Launcher Madgine PRIVATE OgreMadgine)
include_library(Embedded_Client_Launcher Lua PRIVATE lualib)


add_subdirectory(Standalone)