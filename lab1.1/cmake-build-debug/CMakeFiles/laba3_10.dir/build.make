# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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
CMAKE_SOURCE_DIR = /mnt/c/Users/Алсений/CLionProjects/fund_alg/lab1.1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/c/Users/Алсений/CLionProjects/fund_alg/lab1.1/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/laba3_10.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/laba3_10.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/laba3_10.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/laba3_10.dir/flags.make

CMakeFiles/laba3_10.dir/Lab3/lab10/laba3_10.c.o: CMakeFiles/laba3_10.dir/flags.make
CMakeFiles/laba3_10.dir/Lab3/lab10/laba3_10.c.o: /mnt/c/Users/Алсений/CLionProjects/fund_alg/lab1.1/Lab3/lab10/laba3_10.c
CMakeFiles/laba3_10.dir/Lab3/lab10/laba3_10.c.o: CMakeFiles/laba3_10.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/mnt/c/Users/Алсений/CLionProjects/fund_alg/lab1.1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/laba3_10.dir/Lab3/lab10/laba3_10.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/laba3_10.dir/Lab3/lab10/laba3_10.c.o -MF CMakeFiles/laba3_10.dir/Lab3/lab10/laba3_10.c.o.d -o CMakeFiles/laba3_10.dir/Lab3/lab10/laba3_10.c.o -c /mnt/c/Users/Алсений/CLionProjects/fund_alg/lab1.1/Lab3/lab10/laba3_10.c

CMakeFiles/laba3_10.dir/Lab3/lab10/laba3_10.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/laba3_10.dir/Lab3/lab10/laba3_10.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /mnt/c/Users/Алсений/CLionProjects/fund_alg/lab1.1/Lab3/lab10/laba3_10.c > CMakeFiles/laba3_10.dir/Lab3/lab10/laba3_10.c.i

CMakeFiles/laba3_10.dir/Lab3/lab10/laba3_10.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/laba3_10.dir/Lab3/lab10/laba3_10.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /mnt/c/Users/Алсений/CLionProjects/fund_alg/lab1.1/Lab3/lab10/laba3_10.c -o CMakeFiles/laba3_10.dir/Lab3/lab10/laba3_10.c.s

# Object files for target laba3_10
laba3_10_OBJECTS = \
"CMakeFiles/laba3_10.dir/Lab3/lab10/laba3_10.c.o"

# External object files for target laba3_10
laba3_10_EXTERNAL_OBJECTS =

laba3_10: CMakeFiles/laba3_10.dir/Lab3/lab10/laba3_10.c.o
laba3_10: CMakeFiles/laba3_10.dir/build.make
laba3_10: CMakeFiles/laba3_10.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/mnt/c/Users/Алсений/CLionProjects/fund_alg/lab1.1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable laba3_10"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/laba3_10.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/laba3_10.dir/build: laba3_10
.PHONY : CMakeFiles/laba3_10.dir/build

CMakeFiles/laba3_10.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/laba3_10.dir/cmake_clean.cmake
.PHONY : CMakeFiles/laba3_10.dir/clean

CMakeFiles/laba3_10.dir/depend:
	cd /mnt/c/Users/Алсений/CLionProjects/fund_alg/lab1.1/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/c/Users/Алсений/CLionProjects/fund_alg/lab1.1 /mnt/c/Users/Алсений/CLionProjects/fund_alg/lab1.1 /mnt/c/Users/Алсений/CLionProjects/fund_alg/lab1.1/cmake-build-debug /mnt/c/Users/Алсений/CLionProjects/fund_alg/lab1.1/cmake-build-debug /mnt/c/Users/Алсений/CLionProjects/fund_alg/lab1.1/cmake-build-debug/CMakeFiles/laba3_10.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/laba3_10.dir/depend

