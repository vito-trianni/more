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
include examples/CMakeFiles/RngTestSimple.dir/depend.make

# Include the progress variables for this target.
include examples/CMakeFiles/RngTestSimple.dir/progress.make

# Include the compile flags for this target's objects.
include examples/CMakeFiles/RngTestSimple.dir/flags.make

examples/CMakeFiles/RngTestSimple.dir/Rng/RngTestSimple.cpp.o: examples/CMakeFiles/RngTestSimple.dir/flags.make
examples/CMakeFiles/RngTestSimple.dir/Rng/RngTestSimple.cpp.o: examples/Rng/RngTestSimple.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/vtrianni/Work/MORE/Shark/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object examples/CMakeFiles/RngTestSimple.dir/Rng/RngTestSimple.cpp.o"
	cd /home/vtrianni/Work/MORE/Shark/examples && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/RngTestSimple.dir/Rng/RngTestSimple.cpp.o -c /home/vtrianni/Work/MORE/Shark/examples/Rng/RngTestSimple.cpp

examples/CMakeFiles/RngTestSimple.dir/Rng/RngTestSimple.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/RngTestSimple.dir/Rng/RngTestSimple.cpp.i"
	cd /home/vtrianni/Work/MORE/Shark/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/vtrianni/Work/MORE/Shark/examples/Rng/RngTestSimple.cpp > CMakeFiles/RngTestSimple.dir/Rng/RngTestSimple.cpp.i

examples/CMakeFiles/RngTestSimple.dir/Rng/RngTestSimple.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/RngTestSimple.dir/Rng/RngTestSimple.cpp.s"
	cd /home/vtrianni/Work/MORE/Shark/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/vtrianni/Work/MORE/Shark/examples/Rng/RngTestSimple.cpp -o CMakeFiles/RngTestSimple.dir/Rng/RngTestSimple.cpp.s

examples/CMakeFiles/RngTestSimple.dir/Rng/RngTestSimple.cpp.o.requires:
.PHONY : examples/CMakeFiles/RngTestSimple.dir/Rng/RngTestSimple.cpp.o.requires

examples/CMakeFiles/RngTestSimple.dir/Rng/RngTestSimple.cpp.o.provides: examples/CMakeFiles/RngTestSimple.dir/Rng/RngTestSimple.cpp.o.requires
	$(MAKE) -f examples/CMakeFiles/RngTestSimple.dir/build.make examples/CMakeFiles/RngTestSimple.dir/Rng/RngTestSimple.cpp.o.provides.build
.PHONY : examples/CMakeFiles/RngTestSimple.dir/Rng/RngTestSimple.cpp.o.provides

examples/CMakeFiles/RngTestSimple.dir/Rng/RngTestSimple.cpp.o.provides.build: examples/CMakeFiles/RngTestSimple.dir/Rng/RngTestSimple.cpp.o

# Object files for target RngTestSimple
RngTestSimple_OBJECTS = \
"CMakeFiles/RngTestSimple.dir/Rng/RngTestSimple.cpp.o"

# External object files for target RngTestSimple
RngTestSimple_EXTERNAL_OBJECTS =

examples/RngTestSimple: examples/CMakeFiles/RngTestSimple.dir/Rng/RngTestSimple.cpp.o
examples/RngTestSimple: examples/CMakeFiles/RngTestSimple.dir/build.make
examples/RngTestSimple: libshark.so.2.3.0
examples/RngTestSimple: examples/CMakeFiles/RngTestSimple.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable RngTestSimple"
	cd /home/vtrianni/Work/MORE/Shark/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/RngTestSimple.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/CMakeFiles/RngTestSimple.dir/build: examples/RngTestSimple
.PHONY : examples/CMakeFiles/RngTestSimple.dir/build

examples/CMakeFiles/RngTestSimple.dir/requires: examples/CMakeFiles/RngTestSimple.dir/Rng/RngTestSimple.cpp.o.requires
.PHONY : examples/CMakeFiles/RngTestSimple.dir/requires

examples/CMakeFiles/RngTestSimple.dir/clean:
	cd /home/vtrianni/Work/MORE/Shark/examples && $(CMAKE_COMMAND) -P CMakeFiles/RngTestSimple.dir/cmake_clean.cmake
.PHONY : examples/CMakeFiles/RngTestSimple.dir/clean

examples/CMakeFiles/RngTestSimple.dir/depend:
	cd /home/vtrianni/Work/MORE/Shark && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/vtrianni/Work/MORE/Shark /home/vtrianni/Work/MORE/Shark/examples /home/vtrianni/Work/MORE/Shark /home/vtrianni/Work/MORE/Shark/examples /home/vtrianni/Work/MORE/Shark/examples/CMakeFiles/RngTestSimple.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/CMakeFiles/RngTestSimple.dir/depend
