cmake_minimum_required (VERSION 2.6)

cmake_policy(SET CMP0011 OLD)
cmake_policy(SET CMP0020 OLD)
cmake_policy(SET CMP0049 OLD)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/../Madgine-Build/bin)

Project(Maditor)

set(QT_PATH "" CACHE PATH "Path to Qt-find script")

set(CMAKE_MODULE_PATH
	"${QT_PATH}"
	"${CMAKE_CURRENT_LIST_DIR}/CMake"
)

MESSAGE(STATUS "${CMAKE_CURRENT_LIST_DIR}/CMake")

include(PrecompiledHeader)
include(Project)

#Find the QtWidgets library
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Xml REQUIRED)
find_package(QScintilla REQUIRED)
find_package(Qt5Network REQUIRED)

find_package(Madgine REQUIRED)

add_subdirectory(Shared)

add_subdirectory(Launcher)

add_subdirectory(Model)

add_subdirectory(Tools)

add_subdirectory(View)


if (BUILD_TESTS)
	add_subdirectory(test)
endif(BUILD_TESTS)

include_directories(${MADGINE_INCLUDE_DIRS})

add_executable(Maditor main.cpp)
add_dependencies(Maditor Maditor_Launcher ${TOOLS_LIBS})
target_link_libraries(Maditor Maditor_Model Maditor_View)

set(addons "")
# Call the "cat" function for each input file
foreach(TOOL_LIB ${TOOLS_LIBS})
  set(addons ${addons}$<TARGET_FILE:${TOOL_LIB}>\n)
endforeach()

MESSAGE(STATUS ${addons})

file(GENERATE OUTPUT addons.txt CONTENT ${addons})

#install(FILES addons.txt
#        DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG})