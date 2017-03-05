#check cmake version
cmake_minimum_required(VERSION 3.0)

project(01Intro C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(
  ${PROJECT_NAME}.run
  src/main.c
  )

add_executable(
  ${PROJECT_NAME}_merge.run
  src/merge.c
  )
