cmake_minimum_required(VERSION 3.5)

project(gtest-exploration LANGUAGES CXX)

set(CMAKE_PREFIX_PATH $ENV{GTEST_HOME})

find_package(GTest REQUIRED)

include(CTest)

add_executable(${PROJECT_NAME}
  main.cpp
  sample1.cpp
  )

target_link_libraries(${PROJECT_NAME}
  PRIVATE GTest::Main
  )

gtest_discover_tests(${PROJECT_NAME})
