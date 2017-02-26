#check cmake version
cmake_minimum_required(VERSION 3.0)

project(02StaticBuildAndLibrary C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(
  include
  )

# For tracing
#add_definitions(
#  -finstrument-functions
#  )

add_executable(
  ${PROJECT_NAME}.run
  src/fact.c
  src/fibo.c
  src/main.c
  )

add_library(
  fibo SHARED
  src/fibo.c
  )

add_library(
  fact SHARED
  src/fact.c
  )

add_executable(
  ${PROJECT_NAME}_libs.run
  src/main.c
  )

target_link_libraries(
  ${PROJECT_NAME}_libs.run
  fibo
  fact
  )
