cmake_minimum_required(VERSION 3.10)

project(01.BuilderIntro CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(${PROJECT_NAME}
    Builder.cpp
)
