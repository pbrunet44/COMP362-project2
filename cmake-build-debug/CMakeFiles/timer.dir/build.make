# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /snap/clion/69/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /snap/clion/69/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/philip/Desktop/comp362/project02

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/philip/Desktop/comp362/project02/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/timer.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/timer.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/timer.dir/flags.make

CMakeFiles/timer.dir/timer.c.o: CMakeFiles/timer.dir/flags.make
CMakeFiles/timer.dir/timer.c.o: ../timer.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/philip/Desktop/comp362/project02/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/timer.dir/timer.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/timer.dir/timer.c.o   -c /home/philip/Desktop/comp362/project02/timer.c

CMakeFiles/timer.dir/timer.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/timer.dir/timer.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/philip/Desktop/comp362/project02/timer.c > CMakeFiles/timer.dir/timer.c.i

CMakeFiles/timer.dir/timer.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/timer.dir/timer.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/philip/Desktop/comp362/project02/timer.c -o CMakeFiles/timer.dir/timer.c.s

# Object files for target timer
timer_OBJECTS = \
"CMakeFiles/timer.dir/timer.c.o"

# External object files for target timer
timer_EXTERNAL_OBJECTS =

bin/timer: CMakeFiles/timer.dir/timer.c.o
bin/timer: CMakeFiles/timer.dir/build.make
bin/timer: CMakeFiles/timer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/philip/Desktop/comp362/project02/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable bin/timer"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/timer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/timer.dir/build: bin/timer

.PHONY : CMakeFiles/timer.dir/build

CMakeFiles/timer.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/timer.dir/cmake_clean.cmake
.PHONY : CMakeFiles/timer.dir/clean

CMakeFiles/timer.dir/depend:
	cd /home/philip/Desktop/comp362/project02/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/philip/Desktop/comp362/project02 /home/philip/Desktop/comp362/project02 /home/philip/Desktop/comp362/project02/cmake-build-debug /home/philip/Desktop/comp362/project02/cmake-build-debug /home/philip/Desktop/comp362/project02/cmake-build-debug/CMakeFiles/timer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/timer.dir/depend

