cmake_minimum_required(VERSION 3.10)

project(03.BuilderFacets CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(${PROJECT_NAME}
    BuilderFacets.cpp
    Person.cpp
    PersonBuilder.cpp
)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE ${PROJECT_SOURCE_DIR}
)