cmake_minimum_required (VERSION 2.6)

cmake_policy(SET CMP0011 OLD)
cmake_policy(SET CMP0020 OLD)
cmake_policy(SET CMP0049 OLD)

set (CMAKE_CXX_STANDARD 14)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/../bin)

Project(Maditor)

set(QT_PATH "" CACHE PATH "Path to Qt-find script")

set(CMAKE_PREFIX_PATH
  ${CMAKE_PREFIX_PATH}
	"${QT_PATH}"
	"${CMAKE_CURRENT_LIST_DIR}/CMake"
)

set(CMAKE_MODULE_PATH
  ${CMAKE_MODULE_PATH}
	"${CMAKE_CURRENT_LIST_DIR}/CMake"
)

if (MINGW)
	add_compile_options(-Wall -fpermissive)
endif (MINGW)


include(PrecompiledHeader)
include(Project)


#Find the QtWidgets library
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Xml REQUIRED)
find_package(QScintilla REQUIRED)
find_package(Qt5Network REQUIRED)

find_package(Madgine REQUIRED)
find_package(Interfaces REQUIRED)


include_directories(${MADGINE_INCLUDE_DIRS})
include_directories(${INTERFACES_INCLUDE_DIRS})


add_subdirectory(Shared)

add_subdirectory(Launcher)

add_subdirectory(ServerLauncher)

add_subdirectory(Model)

add_subdirectory(Tools)

add_subdirectory(View)


if (BUILD_TESTS)
	add_subdirectory(test)
endif(BUILD_TESTS)

add_executable(Maditor main.cpp)
add_dependencies(Maditor Maditor_Launcher Maditor_Server_Launcher ${TOOLS_LIBS})
target_link_libraries(Maditor Maditor_Model Maditor_View)
