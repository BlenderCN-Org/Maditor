cmake_minimum_required (VERSION 2.6)

cmake_policy(SET CMP0011 OLD)
cmake_policy(SET CMP0049 OLD)


PROJECT(Maditor_Launcher)

load_workspace_package(lua https://github.com/MadManRises/lua.git)


set (sources "")
FolderSources(sources "" "" ApplicationWrapper.cpp ApplicationWrapper.h EmbeddedWrapper.cpp EmbeddedWrapper.h InputWrapper.cpp InputWrapper.h Inspector.cpp Inspector.h maditorlauncherlib.cpp maditorlauncherlib.h maditorlauncherforward.h main_embedded.cpp ModuleLoader.cpp ModuleLoader.h)
	FolderSources(sources "" Util AppStats.cpp AppStats.h LauncherLogListener.cpp LauncherLogListener.h Util.cpp Util.h)	


add_executable(Embedded_Server_Launcher ${sources})
target_link_libraries(Embedded_Server_Launcher PRIVATE Maditor_Shared MadgineServer lualib)


add_executable(Embedded_Client_Launcher ${sources})
target_link_libraries(Embedded_Client_Launcher PRIVATE Maditor_Shared OgreMadgine lualib)


add_subdirectory(Standalone)