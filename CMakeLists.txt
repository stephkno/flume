cmake_minimum_required(VERSION 3.0)
project(flume_demo)

set(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_FLAGS "-Wfatal-errors -g")

add_executable(flume_demo main.cc)
include_directories(${CMAKE_SOURCE_DIR})
