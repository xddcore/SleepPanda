cmake_minimum_required(VERSION 3.14)
project(Buzzer VERSION 1.0.0 LANGUAGES CXX)

add_compile_options(-std=c++11 -Wall -Wconversion -Wextra -pedantic)

set(CMAKE_CXX_STANDARD 11)

include_directories(../config)

add_executable(Buzzer Buzzer.cpp config.h Buzzer.h)

target_link_libraries(Buzzer pigpio)
