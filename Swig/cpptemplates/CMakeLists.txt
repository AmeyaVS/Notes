# This is a CMake example for python

cmake_minimum_required(VERSION 3.8)

project(pair CXX)

find_package(SWIG REQUIRED)
include(${SWIG_USE_FILE})

# added instrumentation option.
add_definitions(
  -finstrument-functions
  )

find_package(PythonLibs)
include_directories(
  ${PYTHON_INCLUDE_PATH}
  include
)

set(CMAKE_SWIG_FLAGS "")

message(STATUS "Site Packages: " ${PYTHON_SITE_PACKAGES})
message(STATUS "Python Modules Libraries: " ${PYTHON_MODULE_PATH})

set_source_files_properties(src/${PROJECT_NAME}.i PROPERTIES CPLUSPLUS ON)
set_source_files_properties(src/${PROJECT_NAME}.i PROPERTIES SWIG_FLAGS "-includeall")
swig_add_library(${PROJECT_NAME} TYPE SHARED LANGUAGE python SOURCES src/${PROJECT_NAME}.i include/pair.h)
swig_link_libraries(${PROJECT_NAME} ${PYTHON_LIBRARIES})
