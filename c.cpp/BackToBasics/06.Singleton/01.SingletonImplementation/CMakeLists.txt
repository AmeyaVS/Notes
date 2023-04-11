cmake_minimum_required(VERSION 3.10)

project(01.SingletonImplementation CXX)

find_package(Boost REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(${PROJECT_NAME}
    SingletonImplementation.cpp
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE Boost::boost
)
