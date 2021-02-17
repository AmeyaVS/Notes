cmake_minimum_required(VERSION 3.5)

project(dpi_example CXX)

set (CMAKE_PREFIX_PATH $ENV{SYSTEMC_HOME})

find_package(verilator HINTS $ENV{VERILATOR_HOME})
find_package(SystemCLanguage CONFIG REQUIRED)

set (CMAKE_CXX_STANDARD ${SystemC_CXX_STANDARD})
set (CMAKE_CXX_EXTENSIONS OFF)
set (CMAKE_EXPORT_COMPILE_COMMANDS ON)
#set (CMAKE_NO_SYSTEM_FROM_IMPORTED ON)
add_executable(
        ${PROJECT_NAME}
        sc_main_verilator.cpp
        add.cpp
        )

verilate(
        ${PROJECT_NAME}
        SOURCES our.v
        SYSTEMC
# For Verilator command line arguments
#        VERILATOR_ARGS --debug
        #--trace
        )

# For debugging using verilator
#target_compile_definitions(
#        ${PROJECT_NAME}
#        PUBLIC VL_DEBUG
#        )

#verilator_link_systemc(${PROJECT_NAME})
# Using SystemC libraries for linking for better debug into SystemC source if need arises.
target_link_libraries(
        ${PROJECT_NAME}
        PRIVATE SystemC::systemc
        )

include (CTest)

# Sanity test for verialted DPI Example
add_test(
        NAME test_dpi_basic_example
        COMMAND ${PROJECT_NAME}
)
