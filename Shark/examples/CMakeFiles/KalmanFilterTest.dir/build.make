# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/vtrianni/Work/MORE/Shark

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/vtrianni/Work/MORE/Shark

# Include any dependencies generated for this target.
include examples/CMakeFiles/KalmanFilterTest.dir/depend.make

# Include the progress variables for this target.
include examples/CMakeFiles/KalmanFilterTest.dir/progress.make

# Include the compile flags for this target's objects.
include examples/CMakeFiles/KalmanFilterTest.dir/flags.make

examples/CMakeFiles/KalmanFilterTest.dir/ReClaM/KalmanFilterTest.cpp.o: examples/CMakeFiles/KalmanFilterTest.dir/flags.make
examples/CMakeFiles/KalmanFilterTest.dir/ReClaM/KalmanFilterTest.cpp.o: examples/ReClaM/KalmanFilterTest.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/vtrianni/Work/MORE/Shark/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object examples/CMakeFiles/KalmanFilterTest.dir/ReClaM/KalmanFilterTest.cpp.o"
	cd /home/vtrianni/Work/MORE/Shark/examples && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/KalmanFilterTest.dir/ReClaM/KalmanFilterTest.cpp.o -c /home/vtrianni/Work/MORE/Shark/examples/ReClaM/KalmanFilterTest.cpp

examples/CMakeFiles/KalmanFilterTest.dir/ReClaM/KalmanFilterTest.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/KalmanFilterTest.dir/ReClaM/KalmanFilterTest.cpp.i"
	cd /home/vtrianni/Work/MORE/Shark/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/vtrianni/Work/MORE/Shark/examples/ReClaM/KalmanFilterTest.cpp > CMakeFiles/KalmanFilterTest.dir/ReClaM/KalmanFilterTest.cpp.i

examples/CMakeFiles/KalmanFilterTest.dir/ReClaM/KalmanFilterTest.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/KalmanFilterTest.dir/ReClaM/KalmanFilterTest.cpp.s"
	cd /home/vtrianni/Work/MORE/Shark/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/vtrianni/Work/MORE/Shark/examples/ReClaM/KalmanFilterTest.cpp -o CMakeFiles/KalmanFilterTest.dir/ReClaM/KalmanFilterTest.cpp.s

examples/CMakeFiles/KalmanFilterTest.dir/ReClaM/KalmanFilterTest.cpp.o.requires:
.PHONY : examples/CMakeFiles/KalmanFilterTest.dir/ReClaM/KalmanFilterTest.cpp.o.requires

examples/CMakeFiles/KalmanFilterTest.dir/ReClaM/KalmanFilterTest.cpp.o.provides: examples/CMakeFiles/KalmanFilterTest.dir/ReClaM/KalmanFilterTest.cpp.o.requires
	$(MAKE) -f examples/CMakeFiles/KalmanFilterTest.dir/build.make examples/CMakeFiles/KalmanFilterTest.dir/ReClaM/KalmanFilterTest.cpp.o.provides.build
.PHONY : examples/CMakeFiles/KalmanFilterTest.dir/ReClaM/KalmanFilterTest.cpp.o.provides

examples/CMakeFiles/KalmanFilterTest.dir/ReClaM/KalmanFilterTest.cpp.o.provides.build: examples/CMakeFiles/KalmanFilterTest.dir/ReClaM/KalmanFilterTest.cpp.o

# Object files for target KalmanFilterTest
KalmanFilterTest_OBJECTS = \
"CMakeFiles/KalmanFilterTest.dir/ReClaM/KalmanFilterTest.cpp.o"

# External object files for target KalmanFilterTest
KalmanFilterTest_EXTERNAL_OBJECTS =

examples/KalmanFilterTest: examples/CMakeFiles/KalmanFilterTest.dir/ReClaM/KalmanFilterTest.cpp.o
examples/KalmanFilterTest: examples/CMakeFiles/KalmanFilterTest.dir/build.make
examples/KalmanFilterTest: libshark.so.2.3.0
examples/KalmanFilterTest: examples/CMakeFiles/KalmanFilterTest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable KalmanFilterTest"
	cd /home/vtrianni/Work/MORE/Shark/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/KalmanFilterTest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/CMakeFiles/KalmanFilterTest.dir/build: examples/KalmanFilterTest
.PHONY : examples/CMakeFiles/KalmanFilterTest.dir/build

examples/CMakeFiles/KalmanFilterTest.dir/requires: examples/CMakeFiles/KalmanFilterTest.dir/ReClaM/KalmanFilterTest.cpp.o.requires
.PHONY : examples/CMakeFiles/KalmanFilterTest.dir/requires

examples/CMakeFiles/KalmanFilterTest.dir/clean:
	cd /home/vtrianni/Work/MORE/Shark/examples && $(CMAKE_COMMAND) -P CMakeFiles/KalmanFilterTest.dir/cmake_clean.cmake
.PHONY : examples/CMakeFiles/KalmanFilterTest.dir/clean

examples/CMakeFiles/KalmanFilterTest.dir/depend:
	cd /home/vtrianni/Work/MORE/Shark && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/vtrianni/Work/MORE/Shark /home/vtrianni/Work/MORE/Shark/examples /home/vtrianni/Work/MORE/Shark /home/vtrianni/Work/MORE/Shark/examples /home/vtrianni/Work/MORE/Shark/examples/CMakeFiles/KalmanFilterTest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/CMakeFiles/KalmanFilterTest.dir/depend

