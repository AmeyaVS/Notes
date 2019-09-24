cmake_minimum_required(VERSION 3.9)

project(SystemCGTest CXX)

include(CTest)

set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(01_basic_test)
add_subdirectory(02_factory_method)
add_subdirectory(03_nand)
add_subdirectory(catch2-fizzbuzz)
add_subdirectory(catch2-bddexample)
