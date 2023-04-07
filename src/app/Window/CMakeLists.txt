cmake_minimum_required(VERSION 3.14)
project(Window VERSION 1.0.0 LANGUAGES CXX)

#print work path
MESSAGE(STATUS "Current path : ${CMAKE_CURRENT_SOURCE_DIR}")

#enable ctest
enable_testing()

#add ./test dir
add_subdirectory(test)
#add unit lib
#MAX30101 LIB
link_libraries(/home/pi/xddcore/SleepPanda/src/app/MAX30101/build/libMAX30101_Static.a)
#Camera LIB
link_libraries(/home/pi/xddcore/SleepPanda/src/app/Camera/build/libCamera_Static.a)
#SoundSensor LIB
link_libraries(/home/pi/xddcore/SleepPanda/src/app/SoundSensor/build/libSoundSensor_Static.a)
#Buzzer LIB
link_libraries(/home/pi/xddcore/SleepPanda/src/app/Buzzer/build/libBuzzer_Static.a)

add_compile_options(-std=c++14 -Wall -Wconversion -Wextra -pedantic)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)

set(LIBSRC Window.cpp)
set(LIBINCLUDE Window.h)
set(LIBSTATIC Window_Static)
set(LIBSTATIC_TARGETS Window_Static-Targets)

#OpenCV
set(opencv_DIR "./") # xxxx目录包含OpenCVConfig.cmake
find_package(OpenCV REQUIRED)	# 找到opencv库

#QT5
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
find_package(Qt5 COMPONENTS Widgets REQUIRED)	# 找到QT5库

add_library(${LIBSTATIC}
  STATIC
  ${LIBSRC}
)

set_target_properties(${LIBSTATIC} PROPERTIES
  PUBLIC_HEADER "${LIBINCLUDE} ${OpenCV_INCLUDE_DIRS}")

target_link_libraries(${LIBSTATIC} pigpio ${OpenCV_LIBRARIES} Qt5::Widgets qwt-qt5 pthread)

install(TARGETS ${LIBSTATIC} EXPORT ${LIBSTATIC_TARGETS}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})