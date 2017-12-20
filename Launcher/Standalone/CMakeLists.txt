

PROJECT(Maditor_Standalone_Launcher)

set (sources "")
FolderSources(sources "" "" StandaloneWrapper.cpp StandaloneWrapper.h main_standalone.cpp)
FolderSources(sources "" ".." ApplicationWrapper.cpp ApplicationWrapper.h EmbeddedWrapper.cpp EmbeddedWrapper.h InputWrapper.cpp InputWrapper.h Inspector.cpp Inspector.h maditorlauncherlib.cpp maditorlauncherlib.h maditorlauncherforward.h ModuleLoader.cpp ModuleLoader.h)
	FolderSources(sources ".." Util AppStats.cpp AppStats.h LauncherLogListener.cpp LauncherLogListener.h Util.cpp Util.h)	


add_definitions(-DCONFIG="${CONFIG}")

add_executable(Standalone_Server_Launcher ${sources})
add_executable(Standalone_Client_Launcher ${sources})


include_targets(Standalone_Server_Launcher Maditor_Shared)
include_library(Standalone_Server_Launcher Madgine PRIVATE MadgineServer)
include_library(Standalone_Server_Launcher Lua PRIVATE lualib)

include_targets(Standalone_Client_Launcher Maditor_Shared)
include_library(Standalone_Client_Launcher Madgine PRIVATE OgreMadgine)
include_library(Standalone_Client_Launcher Lua PRIVATE lualib)