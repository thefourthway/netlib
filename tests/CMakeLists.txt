cmake_minimum_required(VERSION 3.10)
project(tests)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(GTest REQUIRED)

add_executable(run_tests src/test_netlib.cpp)

target_link_libraries(run_tests PRIVATE netlib GTest::GTest GTest::Main)

add_test(NAME tests COMMAND run_tests)
