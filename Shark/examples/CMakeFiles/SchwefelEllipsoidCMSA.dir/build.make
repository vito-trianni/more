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
include examples/CMakeFiles/SchwefelEllipsoidCMSA.dir/depend.make

# Include the progress variables for this target.
include examples/CMakeFiles/SchwefelEllipsoidCMSA.dir/progress.make

# Include the compile flags for this target's objects.
include examples/CMakeFiles/SchwefelEllipsoidCMSA.dir/flags.make

examples/CMakeFiles/SchwefelEllipsoidCMSA.dir/EALib/SchwefelEllipsoidCMSA.cpp.o: examples/CMakeFiles/SchwefelEllipsoidCMSA.dir/flags.make
examples/CMakeFiles/SchwefelEllipsoidCMSA.dir/EALib/SchwefelEllipsoidCMSA.cpp.o: examples/EALib/SchwefelEllipsoidCMSA.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/vtrianni/Work/MORE/Shark/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object examples/CMakeFiles/SchwefelEllipsoidCMSA.dir/EALib/SchwefelEllipsoidCMSA.cpp.o"
	cd /home/vtrianni/Work/MORE/Shark/examples && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/SchwefelEllipsoidCMSA.dir/EALib/SchwefelEllipsoidCMSA.cpp.o -c /home/vtrianni/Work/MORE/Shark/examples/EALib/SchwefelEllipsoidCMSA.cpp

examples/CMakeFiles/SchwefelEllipsoidCMSA.dir/EALib/SchwefelEllipsoidCMSA.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/SchwefelEllipsoidCMSA.dir/EALib/SchwefelEllipsoidCMSA.cpp.i"
	cd /home/vtrianni/Work/MORE/Shark/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/vtrianni/Work/MORE/Shark/examples/EALib/SchwefelEllipsoidCMSA.cpp > CMakeFiles/SchwefelEllipsoidCMSA.dir/EALib/SchwefelEllipsoidCMSA.cpp.i

examples/CMakeFiles/SchwefelEllipsoidCMSA.dir/EALib/SchwefelEllipsoidCMSA.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/SchwefelEllipsoidCMSA.dir/EALib/SchwefelEllipsoidCMSA.cpp.s"
	cd /home/vtrianni/Work/MORE/Shark/examples && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/vtrianni/Work/MORE/Shark/examples/EALib/SchwefelEllipsoidCMSA.cpp -o CMakeFiles/SchwefelEllipsoidCMSA.dir/EALib/SchwefelEllipsoidCMSA.cpp.s

examples/CMakeFiles/SchwefelEllipsoidCMSA.dir/EALib/SchwefelEllipsoidCMSA.cpp.o.requires:
.PHONY : examples/CMakeFiles/SchwefelEllipsoidCMSA.dir/EALib/SchwefelEllipsoidCMSA.cpp.o.requires

examples/CMakeFiles/SchwefelEllipsoidCMSA.dir/EALib/SchwefelEllipsoidCMSA.cpp.o.provides: examples/CMakeFiles/SchwefelEllipsoidCMSA.dir/EALib/SchwefelEllipsoidCMSA.cpp.o.requires
	$(MAKE) -f examples/CMakeFiles/SchwefelEllipsoidCMSA.dir/build.make examples/CMakeFiles/SchwefelEllipsoidCMSA.dir/EALib/SchwefelEllipsoidCMSA.cpp.o.provides.build
.PHONY : examples/CMakeFiles/SchwefelEllipsoidCMSA.dir/EALib/SchwefelEllipsoidCMSA.cpp.o.provides

examples/CMakeFiles/SchwefelEllipsoidCMSA.dir/EALib/SchwefelEllipsoidCMSA.cpp.o.provides.build: examples/CMakeFiles/SchwefelEllipsoidCMSA.dir/EALib/SchwefelEllipsoidCMSA.cpp.o

# Object files for target SchwefelEllipsoidCMSA
SchwefelEllipsoidCMSA_OBJECTS = \
"CMakeFiles/SchwefelEllipsoidCMSA.dir/EALib/SchwefelEllipsoidCMSA.cpp.o"

# External object files for target SchwefelEllipsoidCMSA
SchwefelEllipsoidCMSA_EXTERNAL_OBJECTS =

examples/SchwefelEllipsoidCMSA: examples/CMakeFiles/SchwefelEllipsoidCMSA.dir/EALib/SchwefelEllipsoidCMSA.cpp.o
examples/SchwefelEllipsoidCMSA: examples/CMakeFiles/SchwefelEllipsoidCMSA.dir/build.make
examples/SchwefelEllipsoidCMSA: libshark.so.2.3.0
examples/SchwefelEllipsoidCMSA: examples/CMakeFiles/SchwefelEllipsoidCMSA.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable SchwefelEllipsoidCMSA"
	cd /home/vtrianni/Work/MORE/Shark/examples && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/SchwefelEllipsoidCMSA.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
examples/CMakeFiles/SchwefelEllipsoidCMSA.dir/build: examples/SchwefelEllipsoidCMSA
.PHONY : examples/CMakeFiles/SchwefelEllipsoidCMSA.dir/build

examples/CMakeFiles/SchwefelEllipsoidCMSA.dir/requires: examples/CMakeFiles/SchwefelEllipsoidCMSA.dir/EALib/SchwefelEllipsoidCMSA.cpp.o.requires
.PHONY : examples/CMakeFiles/SchwefelEllipsoidCMSA.dir/requires

examples/CMakeFiles/SchwefelEllipsoidCMSA.dir/clean:
	cd /home/vtrianni/Work/MORE/Shark/examples && $(CMAKE_COMMAND) -P CMakeFiles/SchwefelEllipsoidCMSA.dir/cmake_clean.cmake
.PHONY : examples/CMakeFiles/SchwefelEllipsoidCMSA.dir/clean

examples/CMakeFiles/SchwefelEllipsoidCMSA.dir/depend:
	cd /home/vtrianni/Work/MORE/Shark && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/vtrianni/Work/MORE/Shark /home/vtrianni/Work/MORE/Shark/examples /home/vtrianni/Work/MORE/Shark /home/vtrianni/Work/MORE/Shark/examples /home/vtrianni/Work/MORE/Shark/examples/CMakeFiles/SchwefelEllipsoidCMSA.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : examples/CMakeFiles/SchwefelEllipsoidCMSA.dir/depend

