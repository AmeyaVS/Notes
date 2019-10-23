cmake_minimum_required(VERSION 3.8)
project(02_factory_test)

set(CMAKE_PREFIX_PATH $ENV{SYSTEMC_HOME};$ENV{GTEST_HOME})

message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")

find_package(SystemCLanguage CONFIG REQUIRED)

set (CMAKE_CXX_STANDARD ${SystemC_CXX_STANDARD})

find_package(GTest REQUIRED)

include(CTest)

add_executable(${PROJECT_NAME} main.cpp factory.cpp test.cpp)

target_link_libraries(${PROJECT_NAME} GTest::GTest SystemC::systemc )

add_test(${PROJECT_NAME} ${PROJECT_NAME})

gtest_discover_tests(${PROJECT_NAME})
