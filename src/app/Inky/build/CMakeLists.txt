cmake_minimum_required(VERSION 3.16)

# Define our project
project("inky-cpp")
set(CMAKE_CXX_STANDARD 20)

# Create all the dependecy targets
add_subdirectory("deps")

add_executable( ${PROJECT_NAME} 
                    src/BoundingBox.cpp
                    src/Color.cpp
                    src/Image.cpp
                    src/ImageIO.cpp
                    src/Draw.cpp
                    src/Dither.cpp
                    src/Inky.cpp
                    src/I2CDevice.cpp
                    src/SPIDevice.cpp
                    src/HttpService.cpp
                    src/QRCode.cpp
                    src/main.cpp )

target_include_directories(${PROJECT_NAME} PUBLIC include)

target_link_libraries(${PROJECT_NAME} httplib 
                                      fmt 
                                      gpio-cpp
                                      json 
                                      libjpeg
                                      libpng
                                      magic_enum
                                      qr_code
                                      image_resampler
                                      sigslot
                                      TinyEXIF
                                      broadcom_host)

# Link the c++ filesystem API and pthreads under GCC
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  target_link_libraries(${PROJECT_NAME} stdc++fs pthread)
endif()

add_custom_target(copy_resources ALL)

add_custom_command(TARGET copy_resources POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_directory
                    ${PROJECT_SOURCE_DIR}/resources
                    ${CMAKE_CURRENT_BINARY_DIR}/resources)