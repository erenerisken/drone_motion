# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_SOURCE_DIR = /home/burak/droneTask/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/burak/droneTask/build

# Include any dependencies generated for this target.
include hectorquad/CMakeFiles/goPose.dir/depend.make

# Include the progress variables for this target.
include hectorquad/CMakeFiles/goPose.dir/progress.make

# Include the compile flags for this target's objects.
include hectorquad/CMakeFiles/goPose.dir/flags.make

hectorquad/CMakeFiles/goPose.dir/src/goPose.cpp.o: hectorquad/CMakeFiles/goPose.dir/flags.make
hectorquad/CMakeFiles/goPose.dir/src/goPose.cpp.o: /home/burak/droneTask/src/hectorquad/src/goPose.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/burak/droneTask/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object hectorquad/CMakeFiles/goPose.dir/src/goPose.cpp.o"
	cd /home/burak/droneTask/build/hectorquad && /usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/goPose.dir/src/goPose.cpp.o -c /home/burak/droneTask/src/hectorquad/src/goPose.cpp

hectorquad/CMakeFiles/goPose.dir/src/goPose.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/goPose.dir/src/goPose.cpp.i"
	cd /home/burak/droneTask/build/hectorquad && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/burak/droneTask/src/hectorquad/src/goPose.cpp > CMakeFiles/goPose.dir/src/goPose.cpp.i

hectorquad/CMakeFiles/goPose.dir/src/goPose.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/goPose.dir/src/goPose.cpp.s"
	cd /home/burak/droneTask/build/hectorquad && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/burak/droneTask/src/hectorquad/src/goPose.cpp -o CMakeFiles/goPose.dir/src/goPose.cpp.s

hectorquad/CMakeFiles/goPose.dir/src/goPose.cpp.o.requires:

.PHONY : hectorquad/CMakeFiles/goPose.dir/src/goPose.cpp.o.requires

hectorquad/CMakeFiles/goPose.dir/src/goPose.cpp.o.provides: hectorquad/CMakeFiles/goPose.dir/src/goPose.cpp.o.requires
	$(MAKE) -f hectorquad/CMakeFiles/goPose.dir/build.make hectorquad/CMakeFiles/goPose.dir/src/goPose.cpp.o.provides.build
.PHONY : hectorquad/CMakeFiles/goPose.dir/src/goPose.cpp.o.provides

hectorquad/CMakeFiles/goPose.dir/src/goPose.cpp.o.provides.build: hectorquad/CMakeFiles/goPose.dir/src/goPose.cpp.o


# Object files for target goPose
goPose_OBJECTS = \
"CMakeFiles/goPose.dir/src/goPose.cpp.o"

# External object files for target goPose
goPose_EXTERNAL_OBJECTS =

/home/burak/droneTask/devel/lib/hectorquad/goPose: hectorquad/CMakeFiles/goPose.dir/src/goPose.cpp.o
/home/burak/droneTask/devel/lib/hectorquad/goPose: hectorquad/CMakeFiles/goPose.dir/build.make
/home/burak/droneTask/devel/lib/hectorquad/goPose: /opt/ros/kinetic/lib/libroscpp.so
/home/burak/droneTask/devel/lib/hectorquad/goPose: /usr/lib/x86_64-linux-gnu/libboost_filesystem.so
/home/burak/droneTask/devel/lib/hectorquad/goPose: /usr/lib/x86_64-linux-gnu/libboost_signals.so
/home/burak/droneTask/devel/lib/hectorquad/goPose: /opt/ros/kinetic/lib/librosconsole.so
/home/burak/droneTask/devel/lib/hectorquad/goPose: /opt/ros/kinetic/lib/librosconsole_log4cxx.so
/home/burak/droneTask/devel/lib/hectorquad/goPose: /opt/ros/kinetic/lib/librosconsole_backend_interface.so
/home/burak/droneTask/devel/lib/hectorquad/goPose: /usr/lib/x86_64-linux-gnu/liblog4cxx.so
/home/burak/droneTask/devel/lib/hectorquad/goPose: /usr/lib/x86_64-linux-gnu/libboost_regex.so
/home/burak/droneTask/devel/lib/hectorquad/goPose: /opt/ros/kinetic/lib/libxmlrpcpp.so
/home/burak/droneTask/devel/lib/hectorquad/goPose: /opt/ros/kinetic/lib/libroscpp_serialization.so
/home/burak/droneTask/devel/lib/hectorquad/goPose: /opt/ros/kinetic/lib/librostime.so
/home/burak/droneTask/devel/lib/hectorquad/goPose: /opt/ros/kinetic/lib/libcpp_common.so
/home/burak/droneTask/devel/lib/hectorquad/goPose: /usr/lib/x86_64-linux-gnu/libboost_system.so
/home/burak/droneTask/devel/lib/hectorquad/goPose: /usr/lib/x86_64-linux-gnu/libboost_thread.so
/home/burak/droneTask/devel/lib/hectorquad/goPose: /usr/lib/x86_64-linux-gnu/libboost_chrono.so
/home/burak/droneTask/devel/lib/hectorquad/goPose: /usr/lib/x86_64-linux-gnu/libboost_date_time.so
/home/burak/droneTask/devel/lib/hectorquad/goPose: /usr/lib/x86_64-linux-gnu/libboost_atomic.so
/home/burak/droneTask/devel/lib/hectorquad/goPose: /usr/lib/x86_64-linux-gnu/libpthread.so
/home/burak/droneTask/devel/lib/hectorquad/goPose: /usr/lib/x86_64-linux-gnu/libconsole_bridge.so
/home/burak/droneTask/devel/lib/hectorquad/goPose: hectorquad/CMakeFiles/goPose.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/burak/droneTask/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable /home/burak/droneTask/devel/lib/hectorquad/goPose"
	cd /home/burak/droneTask/build/hectorquad && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/goPose.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
hectorquad/CMakeFiles/goPose.dir/build: /home/burak/droneTask/devel/lib/hectorquad/goPose

.PHONY : hectorquad/CMakeFiles/goPose.dir/build

hectorquad/CMakeFiles/goPose.dir/requires: hectorquad/CMakeFiles/goPose.dir/src/goPose.cpp.o.requires

.PHONY : hectorquad/CMakeFiles/goPose.dir/requires

hectorquad/CMakeFiles/goPose.dir/clean:
	cd /home/burak/droneTask/build/hectorquad && $(CMAKE_COMMAND) -P CMakeFiles/goPose.dir/cmake_clean.cmake
.PHONY : hectorquad/CMakeFiles/goPose.dir/clean

hectorquad/CMakeFiles/goPose.dir/depend:
	cd /home/burak/droneTask/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/burak/droneTask/src /home/burak/droneTask/src/hectorquad /home/burak/droneTask/build /home/burak/droneTask/build/hectorquad /home/burak/droneTask/build/hectorquad/CMakeFiles/goPose.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : hectorquad/CMakeFiles/goPose.dir/depend

