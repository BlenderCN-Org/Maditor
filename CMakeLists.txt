cmake_minimum_required (VERSION 2.6)

cmake_policy(SET CMP0011 OLD)
cmake_policy(SET CMP0020 OLD)
cmake_policy(SET CMP0049 OLD)
cmake_policy(SET CMP0022 NEW)

set (CMAKE_CXX_STANDARD 17)

add_compile_options(/std:c++latest)

add_definitions(-D_HAS_TR1_NAMESPACE)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)

Project(Maditor)

set(QT_PATH "" CACHE PATH "Path to Qt-find script")

set(CMAKE_PREFIX_PATH
  ${CMAKE_PREFIX_PATH}
	"${QT_PATH}"
	"${CMAKE_CURRENT_SOURCE_DIR}"
)

set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
	"${CMAKE_SOURCE_DIR}/cmake"
	
)


include(Workspace)
include(PrecompiledHeader)
include(Project)


#Find the QtWidgets library
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Xml REQUIRED)
find_package(QScintilla REQUIRED)
find_package(Qt5Network REQUIRED)

include_directories(${Maditor_SOURCE_DIR})

add_subdirectory(shared)

add_subdirectory(launcher)

add_subdirectory(model)

add_subdirectory(tools)

add_subdirectory(view)


if (BUILD_TESTS)
	add_subdirectory(test)
endif(BUILD_TESTS)

add_executable(Maditor main.cpp)
add_dependencies(Maditor Embedded_Client_Launcher Embedded_Server_Launcher ${TOOLS_LIBS})
target_link_libraries(Maditor Maditor_Model Maditor_View)




file(REMOVE_RECURSE ${CMAKE_CURRENT_BINARY_DIR}/templates)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/templates)

FILE(GLOB_RECURSE files "templates/*")
FOREACH(item ${files})
	if (item MATCHES "(.*)\\.in")
		FILE(RELATIVE_PATH rel ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_MATCH_1})
		configure_file(${item} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/${rel}) 
	else()
		FILE(COPY ${item} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/templates)
	endif()
ENDFOREACH(item)

FILE(COPY templateproject DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

file(GENERATE OUTPUT ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/addons.txt INPUT ${CMAKE_CURRENT_SOURCE_DIR}/addons.txt.in)

