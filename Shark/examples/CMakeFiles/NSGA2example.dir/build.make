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
include examples/CMakeFiles/NSGA2example.dir/depend.make

# Include the progress variables for this target.
include examples/CMakeFiles/NSGA2example.dir/progress.make

# Include the compile flags for this target's objects.
include examples/CMakeFiles/NSGA2example.dir/flags.make

examples/CMakeFiles/NSGA2example.dir/MOO-EALib/NSGA2example.cpp.o: examples/CMakeFiles/NSGA2example.dir/flags.make
examples/CMakeFiles/NSGA2example.dir/MOO-EALib/NSGA2example.cpp.o: examples/MOO-EALib/NSGA2example.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/vtrianni/Work/MORE/Shark/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object examples/CMakeFiles/NSGA2example.dir/MOO-EALib/NSGA2example.cpp.o"
	cd /home/vtrianni/Work/MORE/Shark/examples && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/NSGA2example.dir/MOO-EALib/NSGA2example.cpp.o -c /home/vtrianni/Work/MORE/Shark/examples/MOO-EALib/NSGA2example.cpp

examples/CMakeFiles/NSGA2example.dir/MOO-EALib/NSGA2example.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/NSGA2example.dir/MOO-EALib/NSGA2example.cpp.i"
	cd /home/vtrianni/Work/MORE/Shark/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/vtrianni/Work/MORE/Shark/examples/MOO-EALib/NSGA2example.cpp > CMakeFiles/NSGA2example.dir/MOO-EALib/NSGA2example.cpp.i

examples/CMakeFiles/NSGA2example.dir/MOO-EALib/NSGA2example.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/NSGA2example.dir/MOO-EALib/NSGA2example.cpp.s"
	cd /home/vtrianni/Work/MORE/Shark/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/vtrianni/Work/MORE/Shark/examples/MOO-EALib/NSGA2example.cpp -o CMakeFiles/NSGA2example.dir/MOO-EALib/NSGA2example.cpp.s

examples/CMakeFiles/NSGA2example.dir/MOO-EALib/NSGA2example.cpp.o.requires:
.PHONY : examples/CMakeFiles/NSGA2example.dir/MOO-EALib/NSGA2example.cpp.o.requires

examples/CMakeFiles/NSGA2example.dir/MOO-EALib/NSGA2example.cpp.o.provides: examples/CMakeFiles/NSGA2example.dir/MOO-EALib/NSGA2example.cpp.o.requires
	$(MAKE) -f examples/CMakeFiles/NSGA2example.dir/build.make examples/CMakeFiles/NSGA2example.dir/MOO-EALib/NSGA2example.cpp.o.provides.build
.PHONY : examples/CMakeFiles/NSGA2example.dir/MOO-EALib/NSGA2example.cpp.o.provides

examples/CMakeFiles/NSGA2example.dir/MOO-EALib/NSGA2example.cpp.o.provides.build: examples/CMakeFiles/NSGA2example.dir/MOO-EALib/NSGA2example.cpp.o

# Object files for target NSGA2example
NSGA2example_OBJECTS = \
"CMakeFiles/NSGA2example.dir/MOO-EALib/NSGA2example.cpp.o"

# External object files for target NSGA2example
NSGA2example_EXTERNAL_OBJECTS =

examples/NSGA2example: examples/CMakeFiles/NSGA2example.dir/MOO-EALib/NSGA2example.cpp.o
examples/NSGA2example: examples/CMakeFiles/NSGA2example.dir/build.make
examples/NSGA2example: libshark.so.2.3.0
examples/NSGA2example: examples/CMakeFiles/NSGA2example.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable NSGA2example"
	cd /home/vtrianni/Work/MORE/Shark/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/NSGA2example.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/CMakeFiles/NSGA2example.dir/build: examples/NSGA2example
.PHONY : examples/CMakeFiles/NSGA2example.dir/build

examples/CMakeFiles/NSGA2example.dir/requires: examples/CMakeFiles/NSGA2example.dir/MOO-EALib/NSGA2example.cpp.o.requires
.PHONY : examples/CMakeFiles/NSGA2example.dir/requires

examples/CMakeFiles/NSGA2example.dir/clean:
	cd /home/vtrianni/Work/MORE/Shark/examples && $(CMAKE_COMMAND) -P CMakeFiles/NSGA2example.dir/cmake_clean.cmake
.PHONY : examples/CMakeFiles/NSGA2example.dir/clean

examples/CMakeFiles/NSGA2example.dir/depend:
	cd /home/vtrianni/Work/MORE/Shark && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/vtrianni/Work/MORE/Shark /home/vtrianni/Work/MORE/Shark/examples /home/vtrianni/Work/MORE/Shark /home/vtrianni/Work/MORE/Shark/examples /home/vtrianni/Work/MORE/Shark/examples/CMakeFiles/NSGA2example.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/CMakeFiles/NSGA2example.dir/depend

