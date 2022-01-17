cmake_minimum_required(VERSION 3.0.0)
project(osef VERSION 0.1.0)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

include(CTest)
enable_testing()

find_package(PkgConfig REQUIRED)
find_package(MySQL REQUIRED)

include_directories(${GTK_INCLUDE_DIRS})
include_directories(${MYSQL_INCLUDE_DIRS})
link_directories(${GTK_LIBRARY_DIRS})

pkg_check_modules(GTK REQUIRED gtk+-3.0)

add_executable(Projet main.c)

target_link_libraries(Projet MySQL::MySQL)
target_link_libraries(Projet ${GTK_LIBRARIES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)