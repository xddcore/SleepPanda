# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/pi/xddcore/SleepPanda/src/app/SoundSensor

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pi/xddcore/SleepPanda/src/app/SoundSensor/build

# Include any dependencies generated for this target.
include test/CMakeFiles/SoundSensor_Test.dir/depend.make

# Include the progress variables for this target.
include test/CMakeFiles/SoundSensor_Test.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/SoundSensor_Test.dir/flags.make

test/CMakeFiles/SoundSensor_Test.dir/SoundSensor_Test.cpp.o: test/CMakeFiles/SoundSensor_Test.dir/flags.make
test/CMakeFiles/SoundSensor_Test.dir/SoundSensor_Test.cpp.o: ../test/SoundSensor_Test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/xddcore/SleepPanda/src/app/SoundSensor/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/SoundSensor_Test.dir/SoundSensor_Test.cpp.o"
	cd /home/pi/xddcore/SleepPanda/src/app/SoundSensor/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/SoundSensor_Test.dir/SoundSensor_Test.cpp.o -c /home/pi/xddcore/SleepPanda/src/app/SoundSensor/test/SoundSensor_Test.cpp

test/CMakeFiles/SoundSensor_Test.dir/SoundSensor_Test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SoundSensor_Test.dir/SoundSensor_Test.cpp.i"
	cd /home/pi/xddcore/SleepPanda/src/app/SoundSensor/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/xddcore/SleepPanda/src/app/SoundSensor/test/SoundSensor_Test.cpp > CMakeFiles/SoundSensor_Test.dir/SoundSensor_Test.cpp.i

test/CMakeFiles/SoundSensor_Test.dir/SoundSensor_Test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SoundSensor_Test.dir/SoundSensor_Test.cpp.s"
	cd /home/pi/xddcore/SleepPanda/src/app/SoundSensor/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/xddcore/SleepPanda/src/app/SoundSensor/test/SoundSensor_Test.cpp -o CMakeFiles/SoundSensor_Test.dir/SoundSensor_Test.cpp.s

# Object files for target SoundSensor_Test
SoundSensor_Test_OBJECTS = \
"CMakeFiles/SoundSensor_Test.dir/SoundSensor_Test.cpp.o"

# External object files for target SoundSensor_Test
SoundSensor_Test_EXTERNAL_OBJECTS =

test/SoundSensor_Test: test/CMakeFiles/SoundSensor_Test.dir/SoundSensor_Test.cpp.o
test/SoundSensor_Test: test/CMakeFiles/SoundSensor_Test.dir/build.make
test/SoundSensor_Test: libSoundSensor_Static.a
test/SoundSensor_Test: ../test/../../../../tools/gtest/rpios_32bit/lib/libgtest.so
test/SoundSensor_Test: test/CMakeFiles/SoundSensor_Test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/pi/xddcore/SleepPanda/src/app/SoundSensor/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable SoundSensor_Test"
	cd /home/pi/xddcore/SleepPanda/src/app/SoundSensor/build/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/SoundSensor_Test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/SoundSensor_Test.dir/build: test/SoundSensor_Test

.PHONY : test/CMakeFiles/SoundSensor_Test.dir/build

test/CMakeFiles/SoundSensor_Test.dir/clean:
	cd /home/pi/xddcore/SleepPanda/src/app/SoundSensor/build/test && $(CMAKE_COMMAND) -P CMakeFiles/SoundSensor_Test.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/SoundSensor_Test.dir/clean

test/CMakeFiles/SoundSensor_Test.dir/depend:
	cd /home/pi/xddcore/SleepPanda/src/app/SoundSensor/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pi/xddcore/SleepPanda/src/app/SoundSensor /home/pi/xddcore/SleepPanda/src/app/SoundSensor/test /home/pi/xddcore/SleepPanda/src/app/SoundSensor/build /home/pi/xddcore/SleepPanda/src/app/SoundSensor/build/test /home/pi/xddcore/SleepPanda/src/app/SoundSensor/build/test/CMakeFiles/SoundSensor_Test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/SoundSensor_Test.dir/depend

