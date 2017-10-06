cmake_minimum_required (VERSION 2.6)

cmake_policy(SET CMP0011 OLD)
cmake_policy(SET CMP0049 OLD)


PROJECT(Maditor_Launcher)


set(lua_include "" CACHE PATH "")
set(lua_lib "" CACHE FILEPATH "")



set (sources "")
FolderSources(sources "" "" ApplicationWrapper.cpp ApplicationWrapper.h InputWrapper.cpp InputWrapper.h Inspector.cpp Inspector.h maditorlauncherlib.cpp maditorlauncherlib.h maditorlauncherforward.h main.cpp ModuleLoader.cpp ModuleLoader.h)
	FolderSources(sources "" Util AppStats.cpp AppStats.h LogListener.cpp LogListener.h Util.cpp Util.h)	

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${lua_include})


add_executable(Server_Launcher ${sources})
add_executable(Client_Launcher ${sources})

#add_precompiled_header(Server_Launcher maditorlauncherlib2.h FORCEINCLUDE SOURCE_CXX maditorlauncherlib.cpp)
#add_precompiled_header(Client_Launcher maditorlauncherlib.h FORCEINCLUDE SOURCE_CXX maditorlauncherlib.cpp)

include_targets(Server_Launcher Maditor_Shared)
include_library(Server_Launcher Madgine MadgineServer)

include_targets(Client_Launcher Maditor_Shared)
include_library(Client_Launcher Madgine OgreMadgine)

target_link_libraries (Server_Launcher PRIVATE ${lua_lib})
target_link_libraries (Client_Launcher PRIVATE ${lua_lib})