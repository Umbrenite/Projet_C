cmake_minimum_required(VERSION 3.0.0)
project(osef VERSION 0.1.0)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
add_definitions( -DCURL_STATICLIB )

include(CTest)
enable_testing()

set(CURL_LIBRARY "-lcurl")
set(CURL_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")

find_package(PkgConfig REQUIRED)
find_package(MySQL REQUIRED)
find_package(CURL REQUIRED)

include_directories(${GTK_INCLUDE_DIRS})
link_directories(${GTK_LIBRARY_DIRS})

include_directories( ${CURL_INCLUDE_DIRS} )
include_directories(${CURL_INCLUDE_DIR})
include_directories(${MYSQL_INCLUDE_DIRS})



pkg_check_modules(GTK REQUIRED gtk+-3.0)

add_executable(Projet main.c)

target_link_libraries(Projet MySQL::MySQL)
target_link_libraries(Projet ${GTK_LIBRARIES})
target_link_libraries(Projet ${CURL_LIBRARIES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)