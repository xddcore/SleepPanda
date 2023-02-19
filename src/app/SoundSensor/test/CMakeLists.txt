cmake_minimum_required(VERSION 3.14)
project(SoundSensor_Test)
#print work path
MESSAGE(STATUS "Current path : ${CMAKE_CURRENT_SOURCE_DIR}")

add_compile_options(-std=c++14 -Wall -Wconversion -Wextra -pedantic)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)

set(GTEST_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../../tools/gtest/rpios_32bit") #

include_directories(${GTEST_PATH}/include)
link_directories(${GTEST_PATH}/lib)


add_executable(SoundSensor_Test SoundSensor_Test.cpp)
target_link_libraries(SoundSensor_Test ${GTEST_PATH}/lib/libgtest.so pigpio SoundSensor_Static)
target_include_directories(SoundSensor_Test PRIVATE ..)

add_test(SoundSensor_Test SoundSensor_Test)
