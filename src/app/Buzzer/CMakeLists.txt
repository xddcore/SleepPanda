cmake_minimum_required(VERSION 3.14)
project(Buzzer VERSION 1.0.0 LANGUAGES CXX)

#print work path
MESSAGE(STATUS "Current path : ${CMAKE_CURRENT_SOURCE_DIR}")

add_compile_options(-std=c++11 -Wall -Wconversion -Wextra -pedantic)

set(CMAKE_CXX_STANDARD 11)

add_executable(Buzzer Buzzer.cpp ../config/config.h Buzzer.h)

target_link_libraries(Buzzer pigpio)
