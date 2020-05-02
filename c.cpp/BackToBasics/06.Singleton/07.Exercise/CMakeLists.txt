cmake_minimum_required(VERSION 3.10)

project(07.Exercise CXX)

set(CMAKE_PREFIX_PATH $ENV{GTEST_HOME})
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(GTest REQUIRED)

include(CTest)

add_executable(${PROJECT_NAME}
    exercise.cpp
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE GTest::GTest GTest::Main
)

gtest_discover_tests(${PROJECT_NAME})