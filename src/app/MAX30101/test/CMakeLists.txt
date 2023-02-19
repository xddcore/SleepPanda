cmake_minimum_required(VERSION 3.14)
project(MAX30101_Test VERSION 1.0.0 LANGUAGES CXX)

#print work path
MESSAGE(STATUS "Current path : ${CMAKE_CURRENT_SOURCE_DIR}")

add_compile_options(-std=c++14 -Wall -Wconversion -Wextra -pedantic)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)

set(LIBSRC MAX30101_Test.cpp)
set(LIBSTATIC MAX30101_Static)
set(LIBTESTNAME MAX30101_Test)

set(GTEST_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../../tools/gtest/rpios_32bit") #

include_directories(${GTEST_PATH}/include)
link_directories(${GTEST_PATH}/lib)


add_executable(${LIBTESTNAME} ${LIBSRC})
target_link_libraries(${LIBTESTNAME} ${LIBSTATIC} ${GTEST_PATH}/lib/libgtest.so pigpio)
target_include_directories(${LIBTESTNAME} PRIVATE ..)

add_test(${LIBTESTNAME} ${LIBTESTNAME})
