cmake_minimum_required(VERSION 3.14)
project(SleepPanda VERSION 1.0.0 LANGUAGES CXX)

#print work path
MESSAGE(STATUS "Current path : ${CMAKE_CURRENT_SOURCE_DIR}")

add_compile_options(-std=c++14 -Wall -Wconversion -Wextra -pedantic)

#add unit lib, and Compile all unit code
add_subdirectory(Buzzer)
add_subdirectory(Camera)
add_subdirectory(MAX30101)
add_subdirectory(SoundSensor)
add_subdirectory(Window)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)

set(LIBSRC SleepPanda.cpp)
set(LIBSTATIC Window_Static)
set(LIBTESTNAME SleepPanda)

#set(GTEST_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../../../../tools/gtest/rpios_32bit") #

#QT5
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt5 COMPONENTS Widgets REQUIRED)	# 找到QT5库

include_directories(${GTEST_PATH}/include)
link_directories(${GTEST_PATH}/lib)


add_executable(${LIBTESTNAME} ${LIBSRC})
target_link_libraries(${LIBTESTNAME} ${LIBSTATIC} pigpio ${OpenCV_LIBRARIES} Qt5::Widgets qwt-qt5 pthread)
target_include_directories(${LIBTESTNAME} PRIVATE ..)

add_test(${LIBTESTNAME} ${LIBTESTNAME})
