cmake_minimum_required (VERSION 2.6)

cmake_policy(SET CMP0011 OLD)
cmake_policy(SET CMP0049 OLD)

set(BOOST_PATH "" CACHE PATH "Path to Boost")


PROJECT(Maditor_Shared)


if (MINGW)
	add_compile_options(-Wall -std=c++14 -fpermissive)
endif (MINGW)

include_directories(${BOOST_PATH})

set (sources "")
FolderSources(sources "" "" AppControl.cpp AppControl.h ApplicationInfo.h errorcodes.cpp errorcodes.h InputInfo.h maditorsharedlib.cpp maditorsharedlib.h maditorsharedforward.h moduleinstance.cpp moduleinstance.h SharedBase.h sharedmemory.cpp sharedmemory.h)
	FolderSources(sources "" IPCManager boostIPCbuffer.cpp boostIPCbuffer.h boostIPCConnection.cpp boostIPCConnection.h boostIPCmanager.cpp boostIPCmanager.h boostIPCServer.cpp boostIPCServer.h boostIPCstream.cpp boostIPCstream.h)

include_directories(${MADGINE_INCLUDE_DIRS} ${Maditor_Shared_SOURCE_DIR})

if (OGRE_FOUND)
  include_directories(${OGRE_INCLUDE_DIRS} ${OGRE_Terrain_INCLUDE_DIRS} ${OGRE_RTShaderSystem_INCLUDE_DIRS})
endif (OGRE_FOUND)

add_library(Maditor_Shared SHARED ${sources})

add_precompiled_header(Maditor_Shared maditorsharedlib.h FORCEINCLUDE SOURCE_CXX maditorsharedlib.cpp)

target_link_libraries(Maditor_Shared Interfaces)

