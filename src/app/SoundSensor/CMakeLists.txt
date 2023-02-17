cmake_minimum_required(VERSION 3.14)
project(SoundSensor VERSION 1.0.0 LANGUAGES CXX)

#print work path
MESSAGE(STATUS "Current path : ${CMAKE_CURRENT_SOURCE_DIR}")

#enable ctest
enable_testing()

#add ./test dir
add_subdirectory(test)

add_compile_options(-std=c++11 -Wall -Wconversion -Wextra -pedantic)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)

set(LIBSRC SoundSensor.cpp)
set(LIBINCLUDE SoundSensor.h)


add_library(SoundSensor_Static
  STATIC
  ${LIBSRC}
)

set_target_properties(SoundSensor_Static PROPERTIES
  PUBLIC_HEADER "${LIBINCLUDE}")

target_link_libraries(SoundSensor_Static pigpio)

install(TARGETS SoundSensor_Static EXPORT SoundSensor_Static-Targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})