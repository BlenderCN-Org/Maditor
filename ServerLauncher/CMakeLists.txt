cmake_minimum_required (VERSION 2.6)

cmake_policy(SET CMP0011 OLD)
cmake_policy(SET CMP0049 OLD)


PROJECT(Maditor_Server_Launcher)


set (sources "")
FolderSources(sources "" "" LogListener.cpp LogListener.h maditorserverlauncherlib.cpp maditorserverlauncherlib.h maditorserverlauncherforward.h main.cpp ModuleLoader.cpp ModuleLoader.h ServerWrapper.cpp ServerWrapper.h)

include_directories(${BOOST_PATH})
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${MADGINE_INCLUDE_DIRS})

add_executable(Maditor_Server_Launcher ${sources})

add_precompiled_header(Maditor_Server_Launcher maditorserverlauncherlib.h FORCEINCLUDE SOURCE_CXX maditorserverlauncherlib.cpp)

target_link_libraries(Maditor_Server_Launcher MadgineServer Maditor_Shared)
