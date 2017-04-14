cmake_minimum_required (VERSION 2.6)

cmake_policy(SET CMP0011 OLD)
cmake_policy(SET CMP0049 OLD)


PROJECT(Maditor_Launcher)


set (sources "")
FolderSources(sources "" "" ApplicationWrapper.cpp ApplicationWrapper.h InputWrapper.cpp InputWrapper.h maditorlauncherlib.cpp maditorlauncherlib.h maditorlauncherforward.h main.cpp ModuleLoader.cpp ModuleLoader.h)
	FolderSources(sources "" Util AppStats.cpp AppStats.h LogListener.cpp LogListener.h Util.cpp Util.h)

include_directories(${BOOST_PATH})
include_directories(${CMAKE_SOURCE_DIR})
include_directories(${MADGINE_INCLUDE_DIRS})
include_directories(${Maditor_Launcher_SOURCE_DIR})

if (OGRE_FOUND)
  include_directories(${OGRE_INCLUDE_DIRS} ${OGRE_Terrain_INCLUDE_DIRS} ${OGRE_RTShaderSystem_INCLUDE_DIRS})
endif (OGRE_FOUND)

add_executable(Maditor_Launcher ${sources})

add_precompiled_header(Maditor_Launcher maditorlauncherlib.h FORCEINCLUDE SOURCE_CXX maditorlauncherlib.cpp)

target_link_libraries(Maditor_Launcher OgreMadgine MadgineServer Maditor_Shared)
