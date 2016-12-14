cmake_minimum_required (VERSION 2.6)

cmake_policy(SET CMP0011 OLD)
cmake_policy(SET CMP0049 OLD)

set(BOOST_PATH "" CACHE PATH "Path to Boost")


PROJECT(Maditor_Shared)

include_directories(${BOOST_PATH})

set (sources "")
FolderSources(sources "" "" AppControl.cpp AppControl.h maditorsharedlib.h maditorsharedforward.h sharedmemory.cpp sharedmemory.h)


include_directories(${MADGINE_INCLUDE_DIRS})

if (OGRE_FOUND)
  include_directories(${OGRE_INCLUDE_DIRS} ${OGRE_Terrain_INCLUDE_DIRS} ${OGRE_RTShaderSystem_INCLUDE_DIRS})
endif (OGRE_FOUND)

add_library(Maditor_Shared SHARED ${sources})

#add_precompiled_header(Maditor src/maditorlib.h FORCEINCLUDE SOURCE_CXX maditorlib.cpp)

target_link_libraries(Maditor_Shared Madgine)

