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
include examples/CMakeFiles/g_inverse_matrix.dir/depend.make

# Include the progress variables for this target.
include examples/CMakeFiles/g_inverse_matrix.dir/progress.make

# Include the compile flags for this target's objects.
include examples/CMakeFiles/g_inverse_matrix.dir/flags.make

examples/CMakeFiles/g_inverse_matrix.dir/LinAlg/g_inverse_matrix.cpp.o: examples/CMakeFiles/g_inverse_matrix.dir/flags.make
examples/CMakeFiles/g_inverse_matrix.dir/LinAlg/g_inverse_matrix.cpp.o: examples/LinAlg/g_inverse_matrix.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/vtrianni/Work/MORE/Shark/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object examples/CMakeFiles/g_inverse_matrix.dir/LinAlg/g_inverse_matrix.cpp.o"
	cd /home/vtrianni/Work/MORE/Shark/examples && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/g_inverse_matrix.dir/LinAlg/g_inverse_matrix.cpp.o -c /home/vtrianni/Work/MORE/Shark/examples/LinAlg/g_inverse_matrix.cpp

examples/CMakeFiles/g_inverse_matrix.dir/LinAlg/g_inverse_matrix.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/g_inverse_matrix.dir/LinAlg/g_inverse_matrix.cpp.i"
	cd /home/vtrianni/Work/MORE/Shark/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/vtrianni/Work/MORE/Shark/examples/LinAlg/g_inverse_matrix.cpp > CMakeFiles/g_inverse_matrix.dir/LinAlg/g_inverse_matrix.cpp.i

examples/CMakeFiles/g_inverse_matrix.dir/LinAlg/g_inverse_matrix.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/g_inverse_matrix.dir/LinAlg/g_inverse_matrix.cpp.s"
	cd /home/vtrianni/Work/MORE/Shark/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/vtrianni/Work/MORE/Shark/examples/LinAlg/g_inverse_matrix.cpp -o CMakeFiles/g_inverse_matrix.dir/LinAlg/g_inverse_matrix.cpp.s

examples/CMakeFiles/g_inverse_matrix.dir/LinAlg/g_inverse_matrix.cpp.o.requires:
.PHONY : examples/CMakeFiles/g_inverse_matrix.dir/LinAlg/g_inverse_matrix.cpp.o.requires

examples/CMakeFiles/g_inverse_matrix.dir/LinAlg/g_inverse_matrix.cpp.o.provides: examples/CMakeFiles/g_inverse_matrix.dir/LinAlg/g_inverse_matrix.cpp.o.requires
	$(MAKE) -f examples/CMakeFiles/g_inverse_matrix.dir/build.make examples/CMakeFiles/g_inverse_matrix.dir/LinAlg/g_inverse_matrix.cpp.o.provides.build
.PHONY : examples/CMakeFiles/g_inverse_matrix.dir/LinAlg/g_inverse_matrix.cpp.o.provides

examples/CMakeFiles/g_inverse_matrix.dir/LinAlg/g_inverse_matrix.cpp.o.provides.build: examples/CMakeFiles/g_inverse_matrix.dir/LinAlg/g_inverse_matrix.cpp.o

# Object files for target g_inverse_matrix
g_inverse_matrix_OBJECTS = \
"CMakeFiles/g_inverse_matrix.dir/LinAlg/g_inverse_matrix.cpp.o"

# External object files for target g_inverse_matrix
g_inverse_matrix_EXTERNAL_OBJECTS =

examples/g_inverse_matrix: examples/CMakeFiles/g_inverse_matrix.dir/LinAlg/g_inverse_matrix.cpp.o
examples/g_inverse_matrix: examples/CMakeFiles/g_inverse_matrix.dir/build.make
examples/g_inverse_matrix: libshark.so.2.3.0
examples/g_inverse_matrix: examples/CMakeFiles/g_inverse_matrix.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable g_inverse_matrix"
	cd /home/vtrianni/Work/MORE/Shark/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/g_inverse_matrix.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/CMakeFiles/g_inverse_matrix.dir/build: examples/g_inverse_matrix
.PHONY : examples/CMakeFiles/g_inverse_matrix.dir/build

examples/CMakeFiles/g_inverse_matrix.dir/requires: examples/CMakeFiles/g_inverse_matrix.dir/LinAlg/g_inverse_matrix.cpp.o.requires
.PHONY : examples/CMakeFiles/g_inverse_matrix.dir/requires

examples/CMakeFiles/g_inverse_matrix.dir/clean:
	cd /home/vtrianni/Work/MORE/Shark/examples && $(CMAKE_COMMAND) -P CMakeFiles/g_inverse_matrix.dir/cmake_clean.cmake
.PHONY : examples/CMakeFiles/g_inverse_matrix.dir/clean

examples/CMakeFiles/g_inverse_matrix.dir/depend:
	cd /home/vtrianni/Work/MORE/Shark && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/vtrianni/Work/MORE/Shark /home/vtrianni/Work/MORE/Shark/examples /home/vtrianni/Work/MORE/Shark /home/vtrianni/Work/MORE/Shark/examples /home/vtrianni/Work/MORE/Shark/examples/CMakeFiles/g_inverse_matrix.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/CMakeFiles/g_inverse_matrix.dir/depend

