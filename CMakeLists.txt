cmake_minimum_required(VERSION 3.10)

set(This Vector-with-allocators-and-iterators)

project(${This})

add_subdirectory(googletest)

add_executable(run_test src/vector.cpp test/test.cc)

target_link_libraries(run_test gtest gtest_main)

enable_testing()

