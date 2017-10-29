#check cmake version
cmake_minimum_required(VERSION 3.0)

project(examples)

add_definitions(
  -ggdb3
#  Tuning to native processor architecture.
#  -mtune=native
#  Instrumenting the code.
#  -finstrument-functions
  )

add_subdirectory(01Intro)
add_subdirectory(02StaticBuildAndLibrary)
add_subdirectory(03SegFault)
add_subdirectory(04NullPtr)
add_subdirectory(05SystemCExample)
