cmake_minimum_required(VERSION 3.14)
project(gtest_demo)
#print work path
MESSAGE(STATUS "Current path : ${CMAKE_CURRENT_SOURCE_DIR}")

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)

set(GTEST_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../tools/gtest/rpios_32bit") #

include_directories(${GTEST_PATH}/include)
link_directories(${GTEST_PATH}/lib)


add_executable(gtest_demo gtest_demo.cpp)
target_link_libraries(gtest_demo ${GTEST_PATH}/lib/libgtest.so)
