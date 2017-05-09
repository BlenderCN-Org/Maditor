cmake_minimum_required (VERSION 2.6)

cmake_policy(SET CMP0011 OLD)
cmake_policy(SET CMP0049 OLD)
cmake_policy(SET CMP0022 NEW)

set(BOOST_PATH "" CACHE PATH "Path to Boost (tested with 1_63)")
if (BOOST_PATH STREQUAL "")
    message (FATAL_ERROR "Assertion failed: Boost is required!")
endif () 

PROJECT(Maditor_Shared)

set (sources "")
FolderSources(sources "" "" AppControl.cpp AppControl.h ApplicationInfo.h errorcodes.cpp errorcodes.h InputInfo.h maditorsharedlib.cpp maditorsharedlib.h maditorsharedforward.h moduleinstance.cpp moduleinstance.h SharedBase.h sharedmemory.cpp sharedmemory.h)
	FolderSources(sources "" IPCManager boostIPCbuffer.cpp boostIPCbuffer.h boostIPCConnection.cpp boostIPCConnection.h boostIPCmanager.cpp boostIPCmanager.h boostIPCServer.cpp boostIPCServer.h boostIPCstream.cpp boostIPCstream.h)

include_directories(${Maditor_Shared_SOURCE_DIR})

add_library(Maditor_Shared SHARED ${sources})
add_precompiled_header(Maditor_Shared maditorsharedlib.h FORCEINCLUDE SOURCE_CXX maditorsharedlib.cpp)

target_include_directories(Maditor_Shared PRIVATE ${BOOST_PATH})

include_library(Maditor_Shared Interfaces Interfaces)





