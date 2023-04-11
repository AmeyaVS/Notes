cmake_minimum_required(VERSION 3.10)

project(04.SingletonLifetimeInDIContainer CXX)

find_package(Boost REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(${PROJECT_NAME}
    SingletonLifetimeInDIContainer.cpp
)

add_executable(${PROJECT_NAME}_2
    SingletonLifetimeInDIContainer2.cpp
)

file(DOWNLOAD "https://raw.githubusercontent.com/boost-experimental/di/cpp14/include/boost/di.hpp" ${PROJECT_SOURCE_DIR}/boost_local/di.hpp)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE boost_local/
)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE Boost::boost
)