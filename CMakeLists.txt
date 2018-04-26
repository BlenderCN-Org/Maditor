cmake_minimum_required (VERSION 2.6)

cmake_policy(SET CMP0011 OLD)
cmake_policy(SET CMP0020 OLD)
cmake_policy(SET CMP0049 OLD)
cmake_policy(SET CMP0022 NEW)

set (CMAKE_CXX_STANDARD 17)

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
include(Testing)

load_workspace_package(Madgine https://github.com/MadManRises/Madgine.git GIT_TAG 57b4880 DEFINITIONS -DBUILD_OGRE:BOOL=FALSE)


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


if (BUILD_TESTING)
	add_subdirectory(test)
endif(BUILD_TESTING)

add_executable(Maditor main.cpp)
add_dependencies(Maditor Embedded_Client_Launcher Embedded_Server_Launcher ${TOOLS_LIBS})
target_link_libraries(Maditor Maditor_Model Maditor_View)





file(GENERATE OUTPUT ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/addons.txt CONTENT "${TOOLS_LIBS}")

file(REMOVE_RECURSE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/templates)
file(REMOVE_RECURSE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/templateproject)

configure_file(Dependencies.cmake.in ${CMAKE_BINARY_DIR}/Dependencies.cmake) 


add_custom_target(CopyTemplates ALL
	COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/templateproject
	COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/templates
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/templateproject ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/templateproject
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/templates ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/templates
	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/Dependencies.cmake ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/templates
)

