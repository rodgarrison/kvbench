# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /root/Dev/tmp/HAMT

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /root/Dev/tmp/HAMT/build

# Include any dependencies generated for this target.
include Test/CMakeFiles/SimplePerfTest.dir/depend.make

# Include the progress variables for this target.
include Test/CMakeFiles/SimplePerfTest.dir/progress.make

# Include the compile flags for this target's objects.
include Test/CMakeFiles/SimplePerfTest.dir/flags.make

Test/CMakeFiles/SimplePerfTest.dir/main.cpp.o: Test/CMakeFiles/SimplePerfTest.dir/flags.make
Test/CMakeFiles/SimplePerfTest.dir/main.cpp.o: ../Test/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/Dev/tmp/HAMT/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object Test/CMakeFiles/SimplePerfTest.dir/main.cpp.o"
	cd /root/Dev/tmp/HAMT/build/Test && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/SimplePerfTest.dir/main.cpp.o -c /root/Dev/tmp/HAMT/Test/main.cpp

Test/CMakeFiles/SimplePerfTest.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SimplePerfTest.dir/main.cpp.i"
	cd /root/Dev/tmp/HAMT/build/Test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /root/Dev/tmp/HAMT/Test/main.cpp > CMakeFiles/SimplePerfTest.dir/main.cpp.i

Test/CMakeFiles/SimplePerfTest.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SimplePerfTest.dir/main.cpp.s"
	cd /root/Dev/tmp/HAMT/build/Test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /root/Dev/tmp/HAMT/Test/main.cpp -o CMakeFiles/SimplePerfTest.dir/main.cpp.s

Test/CMakeFiles/SimplePerfTest.dir/dlmalloc/malloc.c.o: Test/CMakeFiles/SimplePerfTest.dir/flags.make
Test/CMakeFiles/SimplePerfTest.dir/dlmalloc/malloc.c.o: ../Test/dlmalloc/malloc.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/root/Dev/tmp/HAMT/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object Test/CMakeFiles/SimplePerfTest.dir/dlmalloc/malloc.c.o"
	cd /root/Dev/tmp/HAMT/build/Test && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/SimplePerfTest.dir/dlmalloc/malloc.c.o   -c /root/Dev/tmp/HAMT/Test/dlmalloc/malloc.c

Test/CMakeFiles/SimplePerfTest.dir/dlmalloc/malloc.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/SimplePerfTest.dir/dlmalloc/malloc.c.i"
	cd /root/Dev/tmp/HAMT/build/Test && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /root/Dev/tmp/HAMT/Test/dlmalloc/malloc.c > CMakeFiles/SimplePerfTest.dir/dlmalloc/malloc.c.i

Test/CMakeFiles/SimplePerfTest.dir/dlmalloc/malloc.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/SimplePerfTest.dir/dlmalloc/malloc.c.s"
	cd /root/Dev/tmp/HAMT/build/Test && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /root/Dev/tmp/HAMT/Test/dlmalloc/malloc.c -o CMakeFiles/SimplePerfTest.dir/dlmalloc/malloc.c.s

# Object files for target SimplePerfTest
SimplePerfTest_OBJECTS = \
"CMakeFiles/SimplePerfTest.dir/main.cpp.o" \
"CMakeFiles/SimplePerfTest.dir/dlmalloc/malloc.c.o"

# External object files for target SimplePerfTest
SimplePerfTest_EXTERNAL_OBJECTS =

Test/SimplePerfTest: Test/CMakeFiles/SimplePerfTest.dir/main.cpp.o
Test/SimplePerfTest: Test/CMakeFiles/SimplePerfTest.dir/dlmalloc/malloc.c.o
Test/SimplePerfTest: Test/CMakeFiles/SimplePerfTest.dir/build.make
Test/SimplePerfTest: Test/HAMT/libHAMT.a
Test/SimplePerfTest: Test/CMakeFiles/SimplePerfTest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/root/Dev/tmp/HAMT/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable SimplePerfTest"
	cd /root/Dev/tmp/HAMT/build/Test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/SimplePerfTest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
Test/CMakeFiles/SimplePerfTest.dir/build: Test/SimplePerfTest

.PHONY : Test/CMakeFiles/SimplePerfTest.dir/build

Test/CMakeFiles/SimplePerfTest.dir/clean:
	cd /root/Dev/tmp/HAMT/build/Test && $(CMAKE_COMMAND) -P CMakeFiles/SimplePerfTest.dir/cmake_clean.cmake
.PHONY : Test/CMakeFiles/SimplePerfTest.dir/clean

Test/CMakeFiles/SimplePerfTest.dir/depend:
	cd /root/Dev/tmp/HAMT/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /root/Dev/tmp/HAMT /root/Dev/tmp/HAMT/Test /root/Dev/tmp/HAMT/build /root/Dev/tmp/HAMT/build/Test /root/Dev/tmp/HAMT/build/Test/CMakeFiles/SimplePerfTest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Test/CMakeFiles/SimplePerfTest.dir/depend

