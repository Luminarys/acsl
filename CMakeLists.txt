cmake_minimum_required (VERSION 2.6)
project (ACSL)
# The version number.
# set (Tutorial_VERSION_MAJOR 1)
# set (Tutorial_VERSION_MINOR 0)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)
 
include_directories(".")

file(GLOB_RECURSE sources test/*.cc)

add_executable(test_main ${sources})
