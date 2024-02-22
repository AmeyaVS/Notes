cmake_minimum_required(VERSION 3.10)

project(c.cpp CXX)

include(CTest)

add_subdirectory(auto)
#add_subdirectory(BackToBasics)
add_subdirectory(catch2-exploration)
#add_subdirectory(doctest-exploration)
add_subdirectory(gtest-exploration)
add_subdirectory(spdlog-exploration)
add_subdirectory(SystemC-GTestExploration)

