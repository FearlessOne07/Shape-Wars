cmake_minimum_required(VERSION 3.10)
project("Shape-Wars")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_BUILD_TYPE "Debug")

add_subdirectory("${CMAKE_SOURCE_DIR}/vendor/raylib")
set(JSONCPP_WITH_TESTS OFF CACHE BOOL "Disable jsoncpp tests")
add_subdirectory("${CMAKE_SOURCE_DIR}/vendor/jsoncpp")
include_directories(
  "${CMAKE_SOURCE_DIR}/src/"
)
add_subdirectory("${CMAKE_SOURCE_DIR}/src")

