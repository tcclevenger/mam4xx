# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

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
CMAKE_SOURCE_DIR = /home/pabosle/mam4xx

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/pabosle/mam4xx/build-default

# Include any dependencies generated for this target.
include src/tests/CMakeFiles/aero_modes.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include src/tests/CMakeFiles/aero_modes.dir/compiler_depend.make

# Include the progress variables for this target.
include src/tests/CMakeFiles/aero_modes.dir/progress.make

# Include the compile flags for this target's objects.
include src/tests/CMakeFiles/aero_modes.dir/flags.make

src/tests/CMakeFiles/aero_modes.dir/aero_modes_tests.cpp.o: src/tests/CMakeFiles/aero_modes.dir/flags.make
src/tests/CMakeFiles/aero_modes.dir/aero_modes_tests.cpp.o: ../src/tests/aero_modes_tests.cpp
src/tests/CMakeFiles/aero_modes.dir/aero_modes_tests.cpp.o: src/tests/CMakeFiles/aero_modes.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/pabosle/mam4xx/build-default/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/tests/CMakeFiles/aero_modes.dir/aero_modes_tests.cpp.o"
	cd /home/pabosle/mam4xx/build-default/src/tests && /opt/rh/gcc-toolset-11/root/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT src/tests/CMakeFiles/aero_modes.dir/aero_modes_tests.cpp.o -MF CMakeFiles/aero_modes.dir/aero_modes_tests.cpp.o.d -o CMakeFiles/aero_modes.dir/aero_modes_tests.cpp.o -c /home/pabosle/mam4xx/src/tests/aero_modes_tests.cpp

src/tests/CMakeFiles/aero_modes.dir/aero_modes_tests.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/aero_modes.dir/aero_modes_tests.cpp.i"
	cd /home/pabosle/mam4xx/build-default/src/tests && /opt/rh/gcc-toolset-11/root/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/pabosle/mam4xx/src/tests/aero_modes_tests.cpp > CMakeFiles/aero_modes.dir/aero_modes_tests.cpp.i

src/tests/CMakeFiles/aero_modes.dir/aero_modes_tests.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/aero_modes.dir/aero_modes_tests.cpp.s"
	cd /home/pabosle/mam4xx/build-default/src/tests && /opt/rh/gcc-toolset-11/root/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/pabosle/mam4xx/src/tests/aero_modes_tests.cpp -o CMakeFiles/aero_modes.dir/aero_modes_tests.cpp.s

# Object files for target aero_modes
aero_modes_OBJECTS = \
"CMakeFiles/aero_modes.dir/aero_modes_tests.cpp.o"

# External object files for target aero_modes
aero_modes_EXTERNAL_OBJECTS =

src/tests/aero_modes: src/tests/CMakeFiles/aero_modes.dir/aero_modes_tests.cpp.o
src/tests/aero_modes: src/tests/CMakeFiles/aero_modes.dir/build.make
src/tests/aero_modes: ../haero-default/lib64/libekat_test_main.a
src/tests/aero_modes: ../haero-default/lib64/libekat_test_session.a
src/tests/aero_modes: ../haero-default/lib64/libhaero.a
src/tests/aero_modes: ../haero-default/lib64/libhaero.a
src/tests/aero_modes: ../haero-default/lib64/libekat.a
src/tests/aero_modes: ../haero-default/lib64/libspdlogd.a
src/tests/aero_modes: ../haero-default/lib64/libkokkoscontainers.a
src/tests/aero_modes: ../haero-default/lib64/libkokkoscore.a
src/tests/aero_modes: /home/pabosle/openmpi-4.1.4/gcc-11/lib/libmpi.so
src/tests/aero_modes: src/tests/CMakeFiles/aero_modes.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/pabosle/mam4xx/build-default/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable aero_modes"
	cd /home/pabosle/mam4xx/build-default/src/tests && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/aero_modes.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/tests/CMakeFiles/aero_modes.dir/build: src/tests/aero_modes
.PHONY : src/tests/CMakeFiles/aero_modes.dir/build

src/tests/CMakeFiles/aero_modes.dir/clean:
	cd /home/pabosle/mam4xx/build-default/src/tests && $(CMAKE_COMMAND) -P CMakeFiles/aero_modes.dir/cmake_clean.cmake
.PHONY : src/tests/CMakeFiles/aero_modes.dir/clean

src/tests/CMakeFiles/aero_modes.dir/depend:
	cd /home/pabosle/mam4xx/build-default && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/pabosle/mam4xx /home/pabosle/mam4xx/src/tests /home/pabosle/mam4xx/build-default /home/pabosle/mam4xx/build-default/src/tests /home/pabosle/mam4xx/build-default/src/tests/CMakeFiles/aero_modes.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/tests/CMakeFiles/aero_modes.dir/depend

