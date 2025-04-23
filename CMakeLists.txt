cmake_minimum_required(VERSION 3.10)

project(Vector)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(test/external/googletest)

add_executable(Vector source/main.cpp source/vector.cpp)
add_executable(Test src/vector.cpp test/test.cc)

target_link_libraries(Test gtest gtest_main)

enable_testing()

