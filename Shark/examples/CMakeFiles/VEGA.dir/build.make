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
include examples/CMakeFiles/VEGA.dir/depend.make

# Include the progress variables for this target.
include examples/CMakeFiles/VEGA.dir/progress.make

# Include the compile flags for this target's objects.
include examples/CMakeFiles/VEGA.dir/flags.make

examples/CMakeFiles/VEGA.dir/MOO-EALib/VEGA.cpp.o: examples/CMakeFiles/VEGA.dir/flags.make
examples/CMakeFiles/VEGA.dir/MOO-EALib/VEGA.cpp.o: examples/MOO-EALib/VEGA.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/vtrianni/Work/MORE/Shark/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object examples/CMakeFiles/VEGA.dir/MOO-EALib/VEGA.cpp.o"
	cd /home/vtrianni/Work/MORE/Shark/examples && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/VEGA.dir/MOO-EALib/VEGA.cpp.o -c /home/vtrianni/Work/MORE/Shark/examples/MOO-EALib/VEGA.cpp

examples/CMakeFiles/VEGA.dir/MOO-EALib/VEGA.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/VEGA.dir/MOO-EALib/VEGA.cpp.i"
	cd /home/vtrianni/Work/MORE/Shark/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/vtrianni/Work/MORE/Shark/examples/MOO-EALib/VEGA.cpp > CMakeFiles/VEGA.dir/MOO-EALib/VEGA.cpp.i

examples/CMakeFiles/VEGA.dir/MOO-EALib/VEGA.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/VEGA.dir/MOO-EALib/VEGA.cpp.s"
	cd /home/vtrianni/Work/MORE/Shark/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/vtrianni/Work/MORE/Shark/examples/MOO-EALib/VEGA.cpp -o CMakeFiles/VEGA.dir/MOO-EALib/VEGA.cpp.s

examples/CMakeFiles/VEGA.dir/MOO-EALib/VEGA.cpp.o.requires:
.PHONY : examples/CMakeFiles/VEGA.dir/MOO-EALib/VEGA.cpp.o.requires

examples/CMakeFiles/VEGA.dir/MOO-EALib/VEGA.cpp.o.provides: examples/CMakeFiles/VEGA.dir/MOO-EALib/VEGA.cpp.o.requires
	$(MAKE) -f examples/CMakeFiles/VEGA.dir/build.make examples/CMakeFiles/VEGA.dir/MOO-EALib/VEGA.cpp.o.provides.build
.PHONY : examples/CMakeFiles/VEGA.dir/MOO-EALib/VEGA.cpp.o.provides

examples/CMakeFiles/VEGA.dir/MOO-EALib/VEGA.cpp.o.provides.build: examples/CMakeFiles/VEGA.dir/MOO-EALib/VEGA.cpp.o

# Object files for target VEGA
VEGA_OBJECTS = \
"CMakeFiles/VEGA.dir/MOO-EALib/VEGA.cpp.o"

# External object files for target VEGA
VEGA_EXTERNAL_OBJECTS =

examples/VEGA: examples/CMakeFiles/VEGA.dir/MOO-EALib/VEGA.cpp.o
examples/VEGA: examples/CMakeFiles/VEGA.dir/build.make
examples/VEGA: libshark.so.2.3.0
examples/VEGA: examples/CMakeFiles/VEGA.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable VEGA"
	cd /home/vtrianni/Work/MORE/Shark/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/VEGA.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/CMakeFiles/VEGA.dir/build: examples/VEGA
.PHONY : examples/CMakeFiles/VEGA.dir/build

examples/CMakeFiles/VEGA.dir/requires: examples/CMakeFiles/VEGA.dir/MOO-EALib/VEGA.cpp.o.requires
.PHONY : examples/CMakeFiles/VEGA.dir/requires

examples/CMakeFiles/VEGA.dir/clean:
	cd /home/vtrianni/Work/MORE/Shark/examples && $(CMAKE_COMMAND) -P CMakeFiles/VEGA.dir/cmake_clean.cmake
.PHONY : examples/CMakeFiles/VEGA.dir/clean

examples/CMakeFiles/VEGA.dir/depend:
	cd /home/vtrianni/Work/MORE/Shark && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/vtrianni/Work/MORE/Shark /home/vtrianni/Work/MORE/Shark/examples /home/vtrianni/Work/MORE/Shark /home/vtrianni/Work/MORE/Shark/examples /home/vtrianni/Work/MORE/Shark/examples/CMakeFiles/VEGA.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/CMakeFiles/VEGA.dir/depend

