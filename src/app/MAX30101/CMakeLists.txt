cmake_minimum_required(VERSION 3.14)
project(MAX30101 VERSION 1.0.0 LANGUAGES CXX)

#print work path
MESSAGE(STATUS "Current path : ${CMAKE_CURRENT_SOURCE_DIR}")

#enable ctest
enable_testing()

#add ./test dir
add_subdirectory(test)

add_compile_options(-std=c++14 -Wall -Wconversion -Wextra -pedantic)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)

set(LIBSRC MAX30101.cpp)
set(LIBINCLUDE MAX30101.h)
set(LIBSTATIC MAX30101_Static)
set(LIBSTATIC_TARGETS MAX30101_Static-Targets)

add_library(${LIBSTATIC}
  STATIC
  ${LIBSRC}
)

set_target_properties(${LIBSTATIC} PROPERTIES
  PUBLIC_HEADER "${LIBINCLUDE}")

target_link_libraries(${LIBSTATIC} pigpio)

install(TARGETS ${LIBSTATIC} EXPORT ${LIBSTATIC_TARGETS}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})