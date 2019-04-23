#check cmake version
cmake_minimum_required(VERSION 3.0)

project(05SystemCExample CXX)

set(CMAKE_MODULE_PATH $ENV{SYSTEMC_HOME})

find_package(SystemCLanguage CONFIG REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD ${SystemC_CXX_STANDARD})

add_executable(
  ${PROJECT_NAME}.run
  src/main.cpp
  )

include_directories(
  ${PROJECT_NAME}.run
  include
  )

target_link_libraries(
  ${PROJECT_NAME}.run
  SystemC::systemc
  )
