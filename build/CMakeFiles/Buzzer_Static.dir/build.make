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
CMAKE_SOURCE_DIR = /home/pi/xddcore/SleepPanda/src/app/Buzzer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pi/xddcore/SleepPanda/build

# Include any dependencies generated for this target.
include CMakeFiles/Buzzer_Static.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Buzzer_Static.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Buzzer_Static.dir/flags.make

CMakeFiles/Buzzer_Static.dir/Buzzer.cpp.o: CMakeFiles/Buzzer_Static.dir/flags.make
CMakeFiles/Buzzer_Static.dir/Buzzer.cpp.o: /home/pi/xddcore/SleepPanda/src/app/Buzzer/Buzzer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/xddcore/SleepPanda/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Buzzer_Static.dir/Buzzer.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Buzzer_Static.dir/Buzzer.cpp.o -c /home/pi/xddcore/SleepPanda/src/app/Buzzer/Buzzer.cpp

CMakeFiles/Buzzer_Static.dir/Buzzer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Buzzer_Static.dir/Buzzer.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/xddcore/SleepPanda/src/app/Buzzer/Buzzer.cpp > CMakeFiles/Buzzer_Static.dir/Buzzer.cpp.i

CMakeFiles/Buzzer_Static.dir/Buzzer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Buzzer_Static.dir/Buzzer.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/xddcore/SleepPanda/src/app/Buzzer/Buzzer.cpp -o CMakeFiles/Buzzer_Static.dir/Buzzer.cpp.s

# Object files for target Buzzer_Static
Buzzer_Static_OBJECTS = \
"CMakeFiles/Buzzer_Static.dir/Buzzer.cpp.o"

# External object files for target Buzzer_Static
Buzzer_Static_EXTERNAL_OBJECTS =

libBuzzer_Static.a: CMakeFiles/Buzzer_Static.dir/Buzzer.cpp.o
libBuzzer_Static.a: CMakeFiles/Buzzer_Static.dir/build.make
libBuzzer_Static.a: CMakeFiles/Buzzer_Static.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/pi/xddcore/SleepPanda/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libBuzzer_Static.a"
	$(CMAKE_COMMAND) -P CMakeFiles/Buzzer_Static.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Buzzer_Static.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Buzzer_Static.dir/build: libBuzzer_Static.a

.PHONY : CMakeFiles/Buzzer_Static.dir/build

CMakeFiles/Buzzer_Static.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Buzzer_Static.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Buzzer_Static.dir/clean

CMakeFiles/Buzzer_Static.dir/depend:
	cd /home/pi/xddcore/SleepPanda/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pi/xddcore/SleepPanda/src/app/Buzzer /home/pi/xddcore/SleepPanda/src/app/Buzzer /home/pi/xddcore/SleepPanda/build /home/pi/xddcore/SleepPanda/build /home/pi/xddcore/SleepPanda/build/CMakeFiles/Buzzer_Static.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Buzzer_Static.dir/depend

