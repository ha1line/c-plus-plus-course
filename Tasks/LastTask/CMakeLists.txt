cmake_minimum_required(VERSION 3.16)
project(LastTask)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE code_sources ${CMAKE_SOURCE_DIR}/src/*.cpp)
add_executable(LastTask ${code_sources})
