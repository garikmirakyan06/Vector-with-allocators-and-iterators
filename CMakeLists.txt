cmake_minimum_required(VERSION 3.10)

project(Vector)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(test/external/googletest)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_executable(Vector source/main.cpp source/vector.cpp)
add_executable(Test source/vector.cpp test/test.cpp)

target_link_libraries(Test gtest gtest_main)

enable_testing()

