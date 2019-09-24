cmake_minimum_required(VERSION 3.5)

project(catch2-bddexample CXX)

set(CMAKE_PREFIX_PATH $ENV{CATCH_HOME})
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")
message(STATUS "CMAKE_CXX_STANDARD: ${CMAKE_CXX_STANDARD}")

find_package(Catch2 REQUIRED)

add_executable(${PROJECT_NAME} main.cpp fizzbuzz.cpp)

target_link_libraries(${PROJECT_NAME} Catch2::Catch2)

include(CTest)
include(Catch)

catch_discover_tests(${PROJECT_NAME})
