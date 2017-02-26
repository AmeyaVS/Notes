#check cmake version
cmake_minimum_required(VERSION 3.0)

project(01Intro C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# For tracing
#add_definitions(
#  -finstrument-functions
#  )

add_executable(
  ${PROJECT_NAME}.run
  src/main.c
  )
