# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.9

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
CMAKE_COMMAND = /usr/lib/clion/bin/cmake/bin/cmake

# The command to remove a file.
RM = /usr/lib/clion/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/userfs/d/djh566/Documents/EMPR/Term2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/userfs/d/djh566/Documents/EMPR/Term2/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Term2.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Term2.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Term2.dir/flags.make

CMakeFiles/Term2.dir/EMPR.c.o: CMakeFiles/Term2.dir/flags.make
CMakeFiles/Term2.dir/EMPR.c.o: ../EMPR.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/userfs/d/djh566/Documents/EMPR/Term2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/Term2.dir/EMPR.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Term2.dir/EMPR.c.o   -c /home/userfs/d/djh566/Documents/EMPR/Term2/EMPR.c

CMakeFiles/Term2.dir/EMPR.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Term2.dir/EMPR.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/userfs/d/djh566/Documents/EMPR/Term2/EMPR.c > CMakeFiles/Term2.dir/EMPR.c.i

CMakeFiles/Term2.dir/EMPR.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Term2.dir/EMPR.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/userfs/d/djh566/Documents/EMPR/Term2/EMPR.c -o CMakeFiles/Term2.dir/EMPR.c.s

CMakeFiles/Term2.dir/EMPR.c.o.requires:

.PHONY : CMakeFiles/Term2.dir/EMPR.c.o.requires

CMakeFiles/Term2.dir/EMPR.c.o.provides: CMakeFiles/Term2.dir/EMPR.c.o.requires
	$(MAKE) -f CMakeFiles/Term2.dir/build.make CMakeFiles/Term2.dir/EMPR.c.o.provides.build
.PHONY : CMakeFiles/Term2.dir/EMPR.c.o.provides

CMakeFiles/Term2.dir/EMPR.c.o.provides.build: CMakeFiles/Term2.dir/EMPR.c.o


CMakeFiles/Term2.dir/Generator.c.o: CMakeFiles/Term2.dir/flags.make
CMakeFiles/Term2.dir/Generator.c.o: ../Generator.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/userfs/d/djh566/Documents/EMPR/Term2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/Term2.dir/Generator.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Term2.dir/Generator.c.o   -c /home/userfs/d/djh566/Documents/EMPR/Term2/Generator.c

CMakeFiles/Term2.dir/Generator.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Term2.dir/Generator.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/userfs/d/djh566/Documents/EMPR/Term2/Generator.c > CMakeFiles/Term2.dir/Generator.c.i

CMakeFiles/Term2.dir/Generator.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Term2.dir/Generator.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/userfs/d/djh566/Documents/EMPR/Term2/Generator.c -o CMakeFiles/Term2.dir/Generator.c.s

CMakeFiles/Term2.dir/Generator.c.o.requires:

.PHONY : CMakeFiles/Term2.dir/Generator.c.o.requires

CMakeFiles/Term2.dir/Generator.c.o.provides: CMakeFiles/Term2.dir/Generator.c.o.requires
	$(MAKE) -f CMakeFiles/Term2.dir/build.make CMakeFiles/Term2.dir/Generator.c.o.provides.build
.PHONY : CMakeFiles/Term2.dir/Generator.c.o.provides

CMakeFiles/Term2.dir/Generator.c.o.provides.build: CMakeFiles/Term2.dir/Generator.c.o


CMakeFiles/Term2.dir/serial.c.o: CMakeFiles/Term2.dir/flags.make
CMakeFiles/Term2.dir/serial.c.o: ../serial.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/userfs/d/djh566/Documents/EMPR/Term2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/Term2.dir/serial.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Term2.dir/serial.c.o   -c /home/userfs/d/djh566/Documents/EMPR/Term2/serial.c

CMakeFiles/Term2.dir/serial.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Term2.dir/serial.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/userfs/d/djh566/Documents/EMPR/Term2/serial.c > CMakeFiles/Term2.dir/serial.c.i

CMakeFiles/Term2.dir/serial.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Term2.dir/serial.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/userfs/d/djh566/Documents/EMPR/Term2/serial.c -o CMakeFiles/Term2.dir/serial.c.s

CMakeFiles/Term2.dir/serial.c.o.requires:

.PHONY : CMakeFiles/Term2.dir/serial.c.o.requires

CMakeFiles/Term2.dir/serial.c.o.provides: CMakeFiles/Term2.dir/serial.c.o.requires
	$(MAKE) -f CMakeFiles/Term2.dir/build.make CMakeFiles/Term2.dir/serial.c.o.provides.build
.PHONY : CMakeFiles/Term2.dir/serial.c.o.provides

CMakeFiles/Term2.dir/serial.c.o.provides.build: CMakeFiles/Term2.dir/serial.c.o


# Object files for target Term2
Term2_OBJECTS = \
"CMakeFiles/Term2.dir/EMPR.c.o" \
"CMakeFiles/Term2.dir/Generator.c.o" \
"CMakeFiles/Term2.dir/serial.c.o"

# External object files for target Term2
Term2_EXTERNAL_OBJECTS =

Term2: CMakeFiles/Term2.dir/EMPR.c.o
Term2: CMakeFiles/Term2.dir/Generator.c.o
Term2: CMakeFiles/Term2.dir/serial.c.o
Term2: CMakeFiles/Term2.dir/build.make
Term2: CMakeFiles/Term2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/userfs/d/djh566/Documents/EMPR/Term2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C executable Term2"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Term2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Term2.dir/build: Term2

.PHONY : CMakeFiles/Term2.dir/build

CMakeFiles/Term2.dir/requires: CMakeFiles/Term2.dir/EMPR.c.o.requires
CMakeFiles/Term2.dir/requires: CMakeFiles/Term2.dir/Generator.c.o.requires
CMakeFiles/Term2.dir/requires: CMakeFiles/Term2.dir/serial.c.o.requires

.PHONY : CMakeFiles/Term2.dir/requires

CMakeFiles/Term2.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Term2.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Term2.dir/clean

CMakeFiles/Term2.dir/depend:
	cd /home/userfs/d/djh566/Documents/EMPR/Term2/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/userfs/d/djh566/Documents/EMPR/Term2 /home/userfs/d/djh566/Documents/EMPR/Term2 /home/userfs/d/djh566/Documents/EMPR/Term2/cmake-build-debug /home/userfs/d/djh566/Documents/EMPR/Term2/cmake-build-debug /home/userfs/d/djh566/Documents/EMPR/Term2/cmake-build-debug/CMakeFiles/Term2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Term2.dir/depend
