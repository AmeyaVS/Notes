cmake_minimum_required(VERSION 3.8)
project(03_nand_test)

set(CMAKE_PREFIX_PATH $ENV{SYSTEMC_HOME};$ENV{GTEST_HOME})

message(STATUS "CMAKE_PREFIX_PATH: ${CMAKE_PREFIX_PATH}")

find_package(SystemCLanguage CONFIG REQUIRED)

set (CMAKE_CXX_STANDARD ${SystemC_CXX_STANDARD})

find_package(GTest REQUIRED)

enable_testing()

add_executable(${PROJECT_NAME} main.cpp test.cpp nand.cpp) #factory.cpp test.cpp)
target_link_libraries(${PROJECT_NAME} ${GTEST_LIBRARIES} SystemC::systemc )
target_include_directories(${PROJECT_NAME} PRIVATE ${GTEST_INCLUDE_DIRS}
  ${PROJECT_SOURCE_DIR})
add_test(NandGateBasicTest ${PROJECT_NAME})
