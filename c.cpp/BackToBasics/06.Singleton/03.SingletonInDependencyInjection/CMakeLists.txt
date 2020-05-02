cmake_minimum_required(VERSION 3.10)

project(03.SingletonInDependencyInjection CXX)

set(CMAKE_PREFIX_PATH $ENV{GTEST_HOME})

find_package(GTest REQUIRED)
find_package(Boost REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)

add_executable(${PROJECT_NAME}
    SingletonImplementation.cpp
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE Boost::boost
    GTest::GTest
)

gtest_discover_tests(
    ${PROJECT_NAME}
)