cmake_minimum_required(VERSION 3.0.0)
project(tcplib VERSION 0.1.0)

include(CTest)
enable_testing()
add_subdirectory(src)
MESSAGE(STATUS "PROJECT_SOURCE_DIR = ${PROJECT_SOURCE_DIR}")

SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
