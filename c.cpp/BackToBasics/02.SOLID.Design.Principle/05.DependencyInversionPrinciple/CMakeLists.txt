cmake_minimum_required(VERSION 3.10)

project(05.DependencyInversionPrinciple CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(${PROJECT_NAME}
    DIP.cpp
)
