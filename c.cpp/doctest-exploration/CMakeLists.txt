cmake_minimum_required(VERSION 3.5)

project(example_exe VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_PREFIX_PATH $ENV{DOCTEST_HOME})

find_package(doctest REQUIRED)

include(CTest)
include(doctest)

add_executable(${PROJECT_NAME} main.cpp)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_link_libraries(${PROJECT_NAME} PRIVATE doctest::doctest)

doctest_discover_tests(${PROJECT_NAME})
