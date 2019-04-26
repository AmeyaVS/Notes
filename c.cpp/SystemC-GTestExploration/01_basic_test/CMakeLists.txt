cmake_minimum_required(VERSION 3.8)
project(systemc_gtest)

set(CMAKE_PREFIX_PATH $ENV{SYSTEMC_HOME};$ENV{GTEST_HOME})

message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")

find_package(SystemCLanguage CONFIG REQUIRED)

set (CMAKE_CXX_STANDARD ${SystemC_CXX_STANDARD})

find_package(GTest REQUIRED)

enable_testing()

add_executable(systemc_gtest main.cpp)
target_link_libraries(systemc_gtest ${GTEST_LIBRARIES} SystemC::systemc )
target_include_directories(systemc_gtest PRIVATE ${GTEST_INCLUDE_DIRS})
add_test(AllTestsInSystemCGtest systemc_gtest)
