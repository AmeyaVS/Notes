#check cmake version
cmake_minimum_required(VERSION 3.0)

project(05SystemCExample CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(
  include
  $ENV{SYSTEMC_HOME}/include
  )

link_directories(
  $ENV{SYSTEMC_HOME}/lib-linux64
  )

add_executable(
  ${PROJECT_NAME}.run
  src/main.cpp
  )

target_link_libraries(
  ${PROJECT_NAME}.run
  systemc
  )
