# This toolchain file allows a Windows machine with the
# GNU Toolchains Raspberry Pi cross compile setup to build
# RPi binaries painlessly under native Windows
# Download the toolchain here: https://gnutoolchains.com/raspberry/
#
# If it's not installed at C:\SysGCC, then change the paths here and in
# .vscode/cmake-kits.json

# the name of the target operating system
set(CMAKE_SYSTEM_NAME "Linux")
set(CMAKE_SYSTEM_PROCESSOR "armv7")

# which compilers to use for C and C++
set(CMAKE_C_COMPILER   "arm-linux-gnueabihf-gcc")
set(CMAKE_CXX_COMPILER "arm-linux-gnueabihf-g++")

# Allows cross-compilation
set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

# where is the target environment located
set(CMAKE_SYSROOT "C:\\SysGCC\\raspberry\\arm-linux-gnueabihf\\sysroot")
set(CMAKE_FIND_ROOT_PATH  "C:\\SysGCC\\raspberry\\arm-linux-gnueabihf\\sysroot")

# Fix a bug with locating zlib
set(ZLIB_LIBRARY "${CMAKE_SYSROOT}\\lib\\arm-linux-gnueabihf\\libz.a")

# adjust the default behavior of the FIND_XXX() commands:
# search programs in the host environment
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)

# search headers and libraries in the target environment
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
