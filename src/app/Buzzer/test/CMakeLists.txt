cmake_minimum_required(VERSION 3.14)
project(Buzzer_Test)
#print work path
MESSAGE(STATUS "Current path : ${CMAKE_CURRENT_SOURCE_DIR}")

add_compile_options(-std=c++11 -Wall -Wconversion -Wextra -pedantic)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)

set(GTEST_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../../tools/gtest/rpios_32bit") #

include_directories(${GTEST_PATH}/include)
link_directories(${GTEST_PATH}/lib)


add_executable(Buzzer_Test Buzzer_Test.cpp)
target_link_libraries(Buzzer_Test ${GTEST_PATH}/lib/libgtest.so pigpio Buzzer_Static)
target_include_directories(Buzzer_Test PRIVATE ..)

add_test(Buzzer_Test Buzzer_Test)
