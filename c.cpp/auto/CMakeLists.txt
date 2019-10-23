cmake_minimum_required(VERSION 3.5)

project(auto CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(${PROJECT_NAME} auto.cpp)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
