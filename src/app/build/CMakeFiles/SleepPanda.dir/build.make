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
CMAKE_SOURCE_DIR = /home/pi/xddcore/SleepPanda/src/app

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pi/xddcore/SleepPanda/src/app/build

# Include any dependencies generated for this target.
include CMakeFiles/SleepPanda.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/SleepPanda.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/SleepPanda.dir/flags.make

CMakeFiles/SleepPanda.dir/SleepPanda_autogen/mocs_compilation.cpp.o: CMakeFiles/SleepPanda.dir/flags.make
CMakeFiles/SleepPanda.dir/SleepPanda_autogen/mocs_compilation.cpp.o: SleepPanda_autogen/mocs_compilation.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/xddcore/SleepPanda/src/app/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/SleepPanda.dir/SleepPanda_autogen/mocs_compilation.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/SleepPanda.dir/SleepPanda_autogen/mocs_compilation.cpp.o -c /home/pi/xddcore/SleepPanda/src/app/build/SleepPanda_autogen/mocs_compilation.cpp

CMakeFiles/SleepPanda.dir/SleepPanda_autogen/mocs_compilation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SleepPanda.dir/SleepPanda_autogen/mocs_compilation.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/xddcore/SleepPanda/src/app/build/SleepPanda_autogen/mocs_compilation.cpp > CMakeFiles/SleepPanda.dir/SleepPanda_autogen/mocs_compilation.cpp.i

CMakeFiles/SleepPanda.dir/SleepPanda_autogen/mocs_compilation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SleepPanda.dir/SleepPanda_autogen/mocs_compilation.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/xddcore/SleepPanda/src/app/build/SleepPanda_autogen/mocs_compilation.cpp -o CMakeFiles/SleepPanda.dir/SleepPanda_autogen/mocs_compilation.cpp.s

CMakeFiles/SleepPanda.dir/SleepPanda.cpp.o: CMakeFiles/SleepPanda.dir/flags.make
CMakeFiles/SleepPanda.dir/SleepPanda.cpp.o: ../SleepPanda.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pi/xddcore/SleepPanda/src/app/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/SleepPanda.dir/SleepPanda.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/SleepPanda.dir/SleepPanda.cpp.o -c /home/pi/xddcore/SleepPanda/src/app/SleepPanda.cpp

CMakeFiles/SleepPanda.dir/SleepPanda.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SleepPanda.dir/SleepPanda.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pi/xddcore/SleepPanda/src/app/SleepPanda.cpp > CMakeFiles/SleepPanda.dir/SleepPanda.cpp.i

CMakeFiles/SleepPanda.dir/SleepPanda.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SleepPanda.dir/SleepPanda.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pi/xddcore/SleepPanda/src/app/SleepPanda.cpp -o CMakeFiles/SleepPanda.dir/SleepPanda.cpp.s

# Object files for target SleepPanda
SleepPanda_OBJECTS = \
"CMakeFiles/SleepPanda.dir/SleepPanda_autogen/mocs_compilation.cpp.o" \
"CMakeFiles/SleepPanda.dir/SleepPanda.cpp.o"

# External object files for target SleepPanda
SleepPanda_EXTERNAL_OBJECTS =

SleepPanda: CMakeFiles/SleepPanda.dir/SleepPanda_autogen/mocs_compilation.cpp.o
SleepPanda: CMakeFiles/SleepPanda.dir/SleepPanda.cpp.o
SleepPanda: CMakeFiles/SleepPanda.dir/build.make
SleepPanda: Window/libWindow_Static.a
SleepPanda: /usr/lib/arm-linux-gnueabihf/libQt5Widgets.so.5.15.2
SleepPanda: ../MAX30101/build/libMAX30101_Static.a
SleepPanda: ../Camera/build/libCamera_Static.a
SleepPanda: ../SoundSensor/build/libSoundSensor_Static.a
SleepPanda: ../Buzzer/build/libBuzzer_Static.a
SleepPanda: /usr/local/lib/arm-linux-gnueabihf/libopencv_gapi.so.4.7.0
SleepPanda: /usr/local/lib/arm-linux-gnueabihf/libopencv_highgui.so.4.7.0
SleepPanda: /usr/local/lib/arm-linux-gnueabihf/libopencv_ml.so.4.7.0
SleepPanda: /usr/local/lib/arm-linux-gnueabihf/libopencv_objdetect.so.4.7.0
SleepPanda: /usr/local/lib/arm-linux-gnueabihf/libopencv_photo.so.4.7.0
SleepPanda: /usr/local/lib/arm-linux-gnueabihf/libopencv_stitching.so.4.7.0
SleepPanda: /usr/local/lib/arm-linux-gnueabihf/libopencv_video.so.4.7.0
SleepPanda: /usr/local/lib/arm-linux-gnueabihf/libopencv_calib3d.so.4.7.0
SleepPanda: /usr/local/lib/arm-linux-gnueabihf/libopencv_dnn.so.4.7.0
SleepPanda: /usr/local/lib/arm-linux-gnueabihf/libopencv_features2d.so.4.7.0
SleepPanda: /usr/local/lib/arm-linux-gnueabihf/libopencv_flann.so.4.7.0
SleepPanda: /usr/local/lib/arm-linux-gnueabihf/libopencv_videoio.so.4.7.0
SleepPanda: /usr/local/lib/arm-linux-gnueabihf/libopencv_imgcodecs.so.4.7.0
SleepPanda: /usr/local/lib/arm-linux-gnueabihf/libopencv_imgproc.so.4.7.0
SleepPanda: /usr/local/lib/arm-linux-gnueabihf/libopencv_core.so.4.7.0
SleepPanda: /usr/lib/arm-linux-gnueabihf/libQt5Widgets.so.5.15.2
SleepPanda: /usr/lib/arm-linux-gnueabihf/libQt5Gui.so.5.15.2
SleepPanda: /usr/lib/arm-linux-gnueabihf/libQt5Core.so.5.15.2
SleepPanda: /usr/lib/arm-linux-gnueabihf/libQt5Gui.so.5.15.2
SleepPanda: /usr/lib/arm-linux-gnueabihf/libQt5Core.so.5.15.2
SleepPanda: CMakeFiles/SleepPanda.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/pi/xddcore/SleepPanda/src/app/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable SleepPanda"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/SleepPanda.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/SleepPanda.dir/build: SleepPanda

.PHONY : CMakeFiles/SleepPanda.dir/build

CMakeFiles/SleepPanda.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/SleepPanda.dir/cmake_clean.cmake
.PHONY : CMakeFiles/SleepPanda.dir/clean

CMakeFiles/SleepPanda.dir/depend:
	cd /home/pi/xddcore/SleepPanda/src/app/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pi/xddcore/SleepPanda/src/app /home/pi/xddcore/SleepPanda/src/app /home/pi/xddcore/SleepPanda/src/app/build /home/pi/xddcore/SleepPanda/src/app/build /home/pi/xddcore/SleepPanda/src/app/build/CMakeFiles/SleepPanda.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/SleepPanda.dir/depend

