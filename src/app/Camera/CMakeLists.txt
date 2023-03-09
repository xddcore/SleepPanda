cmake_minimum_required(VERSION 3.14)
project(Camera VERSION 1.0.0 LANGUAGES CXX)

#print work path
MESSAGE(STATUS "Current path : ${CMAKE_CURRENT_SOURCE_DIR}")

#enable ctest
enable_testing()

#add ./test dir
add_subdirectory(test)

add_compile_options(-std=c++14 -Wall -Wconversion -Wextra -pedantic)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)

set(LIBSRC Camera.cpp)
set(LIBINCLUDE Camera.h)
set(LIBSTATIC Camera_Static)
set(LIBSTATIC_TARGETS Camera_Static-Targets)

#OpenCV
set(opencv_DIR "./") # xxxx目录包含OpenCVConfig.cmake
find_package(OpenCV REQUIRED)	# 找到opencv库


add_library(${LIBSTATIC}
  STATIC
  ${LIBSRC}
)

set_target_properties(${LIBSTATIC} PROPERTIES
  PUBLIC_HEADER "${LIBINCLUDE} ${OpenCV_INCLUDE_DIRS}")

target_link_libraries(${LIBSTATIC} pigpio ${OpenCV_LIBRARIES})

install(TARGETS ${LIBSTATIC} EXPORT ${LIBSTATIC_TARGETS}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})