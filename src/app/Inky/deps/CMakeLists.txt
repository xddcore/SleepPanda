include(ExternalProject)

# httplib - Cmake, header-only
add_library(httplib INTERFACE)
target_include_directories(httplib SYSTEM INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/cpp-httplib)

# fmt - Cmake, header-only
add_library(fmt INTERFACE)
target_compile_definitions(fmt INTERFACE "FMT_HEADER_ONLY")
target_include_directories(fmt SYSTEM INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/fmt/include)

# gpio-cpp
add_subdirectory("gpio-cpp")

# json - Cmake, header-only
add_library(json INTERFACE)
target_include_directories(json SYSTEM INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/json/include)

# libjpeg-turbo, we build this ourselves mostly for fun
ExternalProject_Add(libjpeg-turbo
  SOURCE_DIR        "${PROJECT_SOURCE_DIR}/deps/libjpeg-turbo"
  BINARY_DIR        "${CMAKE_BINARY_DIR}/libjpeg-turbo_build"
  CMAKE_ARGS        "-DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}"
                    "-DENABLE_SHARED:BOOL=FALSE"
                    "-DENABLE_STATIC:BOOL=TRUE"
                    "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
  INSTALL_COMMAND   ""
  BUILD_BYPRODUCTS  "${CMAKE_BINARY_DIR}/libjpeg-turbo_build/libturbojpeg.a"
)
add_library(libjpeg INTERFACE)
add_dependencies(libjpeg libjpeg-turbo)
target_include_directories(libjpeg INTERFACE "${CMAKE_BINARY_DIR}/libjpeg-turbo_build/"
                                             "${CMAKE_SOURCE_DIR}/deps/libjpeg-turbo/")
target_link_libraries(libjpeg INTERFACE "${CMAKE_BINARY_DIR}/libjpeg-turbo_build/libturbojpeg.a")

# libpng, we build this ourselves mostly for fun
if (APPLE AND CMAKE_SYSTEM_PROCESSOR MATCHES "^arm")
set(PNG_ARM_NEON on CACHE bool "" FORCE)
message("Apple Silicon detected, manually forcing ARM NEON optimizations for libpng")
endif()
option(PNG_SHARED OFF)
option(PNG_TESTS OFF)
option(PNG_EXECUTABLES OFF)
add_subdirectory("libpng")
add_library(libpng INTERFACE)
target_link_libraries(libpng INTERFACE png_static)
target_include_directories(libpng INTERFACE ${PROJECT_SOURCE_DIR}/deps/libpng
                                            ${CMAKE_CURRENT_BINARY_DIR}/libpng)

# Magic Enum
add_library(magic_enum INTERFACE)
target_include_directories(magic_enum SYSTEM INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/magic_enum/include)

# QR-Code-generator
add_library(qr_code ${CMAKE_CURRENT_SOURCE_DIR}/QR-Code-generator/cpp/qrcodegen.cpp)
target_include_directories(qr_code SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/QR-Code-generator/cpp)

# single-header-image-resampler
add_library(image_resampler INTERFACE)
target_include_directories(image_resampler SYSTEM INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/single-header-image-resampler)

# sigslot
if(NOT TARGET sigslot)
  add_library(sigslot INTERFACE)
  target_include_directories(sigslot SYSTEM INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/sigslot/include)
endif()

# TinyEXIF
add_library(TinyEXIF ${CMAKE_CURRENT_SOURCE_DIR}/TinyEXIF/TinyEXIF.cpp)
target_compile_definitions(TinyEXIF PUBLIC "TINYEXIF_NO_XMP_SUPPORT")
target_include_directories(TinyEXIF SYSTEM PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/TinyEXIF)

# Broadcom Host
add_library(broadcom_host INTERFACE) 
find_file(BCM_HOST_PATH NAMES "bcm_host.h")
if (BCM_HOST_PATH)
    message("BCM host header: ${BCM_HOST_PATH}")
    target_link_libraries(broadcom_host INTERFACE bcm_host)
else()
    message("BCM host header: Not found! Setting compile def SIMULATE_PI_HARDWARE...")
    target_compile_definitions(broadcom_host INTERFACE "SIMULATE_PI_HARDWARE")  
endif()

# openssl is not used at this time, but if it were, this is how to include it
# if (APPLE)
#     set (OPENSSL_ROOT_DIR /opt/homebrew/opt/openssl@3)
# endif()
# find_package(OpenSSL REQUIRED)
# Then use it like this in the main cmake file:
# target_link_libraries(${PROJECT_NAME} OpenSSL::SSL OpenSSL::Crypto)
