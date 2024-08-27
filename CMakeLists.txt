cmake_minimum_required(VERSION 3.10)
project(netlib VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(netlib)
add_subdirectory(tests)
add_subdirectory(sandbox)

enable_testing()
