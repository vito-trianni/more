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
include examples/CMakeFiles/rank_test.dir/depend.make

# Include the progress variables for this target.
include examples/CMakeFiles/rank_test.dir/progress.make

# Include the compile flags for this target's objects.
include examples/CMakeFiles/rank_test.dir/flags.make

examples/CMakeFiles/rank_test.dir/LinAlg/rank_test.cpp.o: examples/CMakeFiles/rank_test.dir/flags.make
examples/CMakeFiles/rank_test.dir/LinAlg/rank_test.cpp.o: examples/LinAlg/rank_test.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/vtrianni/Work/MORE/Shark/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object examples/CMakeFiles/rank_test.dir/LinAlg/rank_test.cpp.o"
	cd /home/vtrianni/Work/MORE/Shark/examples && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/rank_test.dir/LinAlg/rank_test.cpp.o -c /home/vtrianni/Work/MORE/Shark/examples/LinAlg/rank_test.cpp

examples/CMakeFiles/rank_test.dir/LinAlg/rank_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rank_test.dir/LinAlg/rank_test.cpp.i"
	cd /home/vtrianni/Work/MORE/Shark/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/vtrianni/Work/MORE/Shark/examples/LinAlg/rank_test.cpp > CMakeFiles/rank_test.dir/LinAlg/rank_test.cpp.i

examples/CMakeFiles/rank_test.dir/LinAlg/rank_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rank_test.dir/LinAlg/rank_test.cpp.s"
	cd /home/vtrianni/Work/MORE/Shark/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/vtrianni/Work/MORE/Shark/examples/LinAlg/rank_test.cpp -o CMakeFiles/rank_test.dir/LinAlg/rank_test.cpp.s

examples/CMakeFiles/rank_test.dir/LinAlg/rank_test.cpp.o.requires:
.PHONY : examples/CMakeFiles/rank_test.dir/LinAlg/rank_test.cpp.o.requires

examples/CMakeFiles/rank_test.dir/LinAlg/rank_test.cpp.o.provides: examples/CMakeFiles/rank_test.dir/LinAlg/rank_test.cpp.o.requires
	$(MAKE) -f examples/CMakeFiles/rank_test.dir/build.make examples/CMakeFiles/rank_test.dir/LinAlg/rank_test.cpp.o.provides.build
.PHONY : examples/CMakeFiles/rank_test.dir/LinAlg/rank_test.cpp.o.provides

examples/CMakeFiles/rank_test.dir/LinAlg/rank_test.cpp.o.provides.build: examples/CMakeFiles/rank_test.dir/LinAlg/rank_test.cpp.o

# Object files for target rank_test
rank_test_OBJECTS = \
"CMakeFiles/rank_test.dir/LinAlg/rank_test.cpp.o"

# External object files for target rank_test
rank_test_EXTERNAL_OBJECTS =

examples/rank_test: examples/CMakeFiles/rank_test.dir/LinAlg/rank_test.cpp.o
examples/rank_test: examples/CMakeFiles/rank_test.dir/build.make
examples/rank_test: libshark.so.2.3.0
examples/rank_test: examples/CMakeFiles/rank_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable rank_test"
	cd /home/vtrianni/Work/MORE/Shark/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/rank_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/CMakeFiles/rank_test.dir/build: examples/rank_test
.PHONY : examples/CMakeFiles/rank_test.dir/build

examples/CMakeFiles/rank_test.dir/requires: examples/CMakeFiles/rank_test.dir/LinAlg/rank_test.cpp.o.requires
.PHONY : examples/CMakeFiles/rank_test.dir/requires

examples/CMakeFiles/rank_test.dir/clean:
	cd /home/vtrianni/Work/MORE/Shark/examples && $(CMAKE_COMMAND) -P CMakeFiles/rank_test.dir/cmake_clean.cmake
.PHONY : examples/CMakeFiles/rank_test.dir/clean

examples/CMakeFiles/rank_test.dir/depend:
	cd /home/vtrianni/Work/MORE/Shark && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/vtrianni/Work/MORE/Shark /home/vtrianni/Work/MORE/Shark/examples /home/vtrianni/Work/MORE/Shark /home/vtrianni/Work/MORE/Shark/examples /home/vtrianni/Work/MORE/Shark/examples/CMakeFiles/rank_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/CMakeFiles/rank_test.dir/depend

