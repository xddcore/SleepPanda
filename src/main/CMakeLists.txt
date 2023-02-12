cmake_minimum_required(VERSION 3.14)
project(src)

set(CMAKE_CXX_STANDARD 11)

add_executable(src Buzzer.cpp config.h Buzzer.h)
