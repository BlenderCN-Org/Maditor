

PROJECT(Maditor_Standalone_Launcher)

set (sources "")
FolderSources(sources "" "" StandaloneWrapper.cpp StandaloneWrapper.h main_standalone.cpp)
FolderSources(sources "" ".." ApplicationWrapper.cpp ApplicationWrapper.h EmbeddedWrapper.cpp EmbeddedWrapper.h InputWrapper.cpp InputWrapper.h Inspector.cpp Inspector.h maditorlauncherlib.cpp maditorlauncherlib.h maditorlauncherforward.h ModuleLoader.cpp ModuleLoader.h)
	FolderSources(sources ".." Util AppStats.cpp AppStats.h LauncherLogListener.cpp LauncherLogListener.h Util.cpp Util.h)	


add_definitions(-DCONFIG="${CONFIG}")

add_executable(Standalone_Server_Launcher ${sources})
target_link_libraries(Standalone_Server_Launcher PRIVATE Maditor_Shared MadgineServer lualib)

add_executable(Standalone_Client_Launcher ${sources})
target_link_libraries(Standalone_Client_Launcher PRIVATE Maditor_Shared OgreMadgine lualib)
