cmake_minimum_required (VERSION 2.6)

cmake_policy(SET CMP0011 OLD)
cmake_policy(SET CMP0020 OLD)
cmake_policy(SET CMP0049 OLD)
cmake_policy(SET CMP0022 NEW)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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
include(Packaging)



load_workspace_package(Madgine https://github.com/MadManRises/Madgine.git GIT_TAG 6afc306 DEFINITIONS -DBUILD_CLIENT:BOOL=FALSE)


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





file(GENERATE OUTPUT ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/addons.txt CONTENT "${TOOLS_BINS}")

file(REMOVE_RECURSE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/templates)
file(REMOVE_RECURSE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/templateproject)

configure_file(Dependencies.cmake.in ${CMAKE_BINARY_DIR}/Dependencies.cmake) 


add_custom_command(TARGET Maditor POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/templateproject
	COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/templates
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/templateproject ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/templateproject
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/templates ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/templates
	COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/Dependencies.cmake ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/templates
)


# Windows specific build steps
if(WIN32)
    # Run winddeployqt if it can be found
    find_program(WINDEPLOYQT_EXECUTABLE NAMES windeployqt HINTS ${QT_PATH} ENV QTDIR PATH_SUFFIXES bin)
    add_custom_command(TARGET Maditor POST_BUILD
    COMMAND ${WINDEPLOYQT_EXECUTABLE} $<TARGET_FILE:Maditor> --dir ${CMAKE_BINARY_DIR}/qt_dep/bin)

	install(DIRECTORY ${CMAKE_BINARY_DIR}/qt_dep/bin DESTINATION .)
endif()

install(FILES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG}/addons.txt DESTINATION bin)

install_to_workspace(Maditor Maditor)
export_to_workspace(Maditor)


enable_packaging()