cmake_minimum_required(VERSION 3.14)
project(gtest_demo)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)

set(GTEST_PATH "../../../tools/gtest/")

include_directories(${GTEST_PATH}/include)
link_directories(${GTEST_PATH}/lib)

target_link_libraries(gtest_demo ${GTEST_PATH}/lib/libgtest.so)

add_executable(gtest_demo gtest_demo.cpp)