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
include examples/CMakeFiles/fonMO-CMA.dir/depend.make

# Include the progress variables for this target.
include examples/CMakeFiles/fonMO-CMA.dir/progress.make

# Include the compile flags for this target's objects.
include examples/CMakeFiles/fonMO-CMA.dir/flags.make

examples/CMakeFiles/fonMO-CMA.dir/MOO-EALib/fonMO-CMA.cpp.o: examples/CMakeFiles/fonMO-CMA.dir/flags.make
examples/CMakeFiles/fonMO-CMA.dir/MOO-EALib/fonMO-CMA.cpp.o: examples/MOO-EALib/fonMO-CMA.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/vtrianni/Work/MORE/Shark/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object examples/CMakeFiles/fonMO-CMA.dir/MOO-EALib/fonMO-CMA.cpp.o"
	cd /home/vtrianni/Work/MORE/Shark/examples && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/fonMO-CMA.dir/MOO-EALib/fonMO-CMA.cpp.o -c /home/vtrianni/Work/MORE/Shark/examples/MOO-EALib/fonMO-CMA.cpp

examples/CMakeFiles/fonMO-CMA.dir/MOO-EALib/fonMO-CMA.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fonMO-CMA.dir/MOO-EALib/fonMO-CMA.cpp.i"
	cd /home/vtrianni/Work/MORE/Shark/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/vtrianni/Work/MORE/Shark/examples/MOO-EALib/fonMO-CMA.cpp > CMakeFiles/fonMO-CMA.dir/MOO-EALib/fonMO-CMA.cpp.i

examples/CMakeFiles/fonMO-CMA.dir/MOO-EALib/fonMO-CMA.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fonMO-CMA.dir/MOO-EALib/fonMO-CMA.cpp.s"
	cd /home/vtrianni/Work/MORE/Shark/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/vtrianni/Work/MORE/Shark/examples/MOO-EALib/fonMO-CMA.cpp -o CMakeFiles/fonMO-CMA.dir/MOO-EALib/fonMO-CMA.cpp.s

examples/CMakeFiles/fonMO-CMA.dir/MOO-EALib/fonMO-CMA.cpp.o.requires:
.PHONY : examples/CMakeFiles/fonMO-CMA.dir/MOO-EALib/fonMO-CMA.cpp.o.requires

examples/CMakeFiles/fonMO-CMA.dir/MOO-EALib/fonMO-CMA.cpp.o.provides: examples/CMakeFiles/fonMO-CMA.dir/MOO-EALib/fonMO-CMA.cpp.o.requires
	$(MAKE) -f examples/CMakeFiles/fonMO-CMA.dir/build.make examples/CMakeFiles/fonMO-CMA.dir/MOO-EALib/fonMO-CMA.cpp.o.provides.build
.PHONY : examples/CMakeFiles/fonMO-CMA.dir/MOO-EALib/fonMO-CMA.cpp.o.provides

examples/CMakeFiles/fonMO-CMA.dir/MOO-EALib/fonMO-CMA.cpp.o.provides.build: examples/CMakeFiles/fonMO-CMA.dir/MOO-EALib/fonMO-CMA.cpp.o

# Object files for target fonMO-CMA
fonMO__CMA_OBJECTS = \
"CMakeFiles/fonMO-CMA.dir/MOO-EALib/fonMO-CMA.cpp.o"

# External object files for target fonMO-CMA
fonMO__CMA_EXTERNAL_OBJECTS =

examples/fonMO-CMA: examples/CMakeFiles/fonMO-CMA.dir/MOO-EALib/fonMO-CMA.cpp.o
examples/fonMO-CMA: examples/CMakeFiles/fonMO-CMA.dir/build.make
examples/fonMO-CMA: libshark.so.2.3.0
examples/fonMO-CMA: examples/CMakeFiles/fonMO-CMA.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable fonMO-CMA"
	cd /home/vtrianni/Work/MORE/Shark/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/fonMO-CMA.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/CMakeFiles/fonMO-CMA.dir/build: examples/fonMO-CMA
.PHONY : examples/CMakeFiles/fonMO-CMA.dir/build

examples/CMakeFiles/fonMO-CMA.dir/requires: examples/CMakeFiles/fonMO-CMA.dir/MOO-EALib/fonMO-CMA.cpp.o.requires
.PHONY : examples/CMakeFiles/fonMO-CMA.dir/requires

examples/CMakeFiles/fonMO-CMA.dir/clean:
	cd /home/vtrianni/Work/MORE/Shark/examples && $(CMAKE_COMMAND) -P CMakeFiles/fonMO-CMA.dir/cmake_clean.cmake
.PHONY : examples/CMakeFiles/fonMO-CMA.dir/clean

examples/CMakeFiles/fonMO-CMA.dir/depend:
	cd /home/vtrianni/Work/MORE/Shark && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/vtrianni/Work/MORE/Shark /home/vtrianni/Work/MORE/Shark/examples /home/vtrianni/Work/MORE/Shark /home/vtrianni/Work/MORE/Shark/examples /home/vtrianni/Work/MORE/Shark/examples/CMakeFiles/fonMO-CMA.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/CMakeFiles/fonMO-CMA.dir/depend

