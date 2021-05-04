cmake_minimum_required(VERSION 3.5)

project(basic_spdlog CXX)

set(CMAKE_PREFIX_PATH "$ENV{HOME}/apps/spdlog")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(spdlog REQUIRED)

add_executable(${PROJECT_NAME}
        basic_logger.cpp
        )

target_link_libraries(
        ${PROJECT_NAME}
        PRIVATE spdlog::spdlog
        )

# ---------------------------------------------------------------------------------------
# Example of using header-only library
# ---------------------------------------------------------------------------------------
if(SPDLOG_BUILD_EXAMPLE_HO)
    add_executable(example_header_only example.cpp)
    target_link_libraries(example_header_only PRIVATE spdlog::spdlog_header_only)
endif()
