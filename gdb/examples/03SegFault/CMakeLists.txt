# check cmake version
cmake_minimum_required(VERSION 3.0)

project(03SegFault CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(
  ${PROJECT_NAME}.run
  main.cc
  )
