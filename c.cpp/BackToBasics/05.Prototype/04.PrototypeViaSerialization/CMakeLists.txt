cmake_minimum_required(VERSION 3.10)

project(04.PrototypeViaSerialization CXX)

find_package(Boost REQUIRED COMPONENTS serialization)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(${PROJECT_NAME}
    RecordKeeping.cpp
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE Boost::serialization
)
