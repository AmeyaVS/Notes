# check cmake version
cmake_minimum_required(VERSION 3.0)

project(04NullPtr C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(
  ${PROJECT_NAME}.run
  main.c
  )
