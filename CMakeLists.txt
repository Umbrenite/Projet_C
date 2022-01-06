cmake_minimum_required(VERSION 3.0.0)
project(osef VERSION 0.1.0)

include(CTest)
enable_testing()

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk+-3.0)
include_directories(${GTK_INCLUDE_DIRS})
link_directories(${GTK_LIBRARY_DIRS})

add_executable(Projet main.c)

target_link_libraries(Projet ${GTK_LIBRARIES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)