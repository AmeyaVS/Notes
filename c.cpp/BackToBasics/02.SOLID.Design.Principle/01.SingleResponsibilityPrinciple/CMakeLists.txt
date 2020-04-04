cmake_minimum_required(VERSION 3.5)

project(01.SingleResponsibilityPrinciple CXX)

find_package(Boost REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(${PROJECT_NAME}
    SRP.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE Boost::boost
)