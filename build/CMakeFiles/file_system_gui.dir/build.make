# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

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
CMAKE_SOURCE_DIR = /home/jm/1st_project_2020136085

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jm/1st_project_2020136085/build

# Include any dependencies generated for this target.
include CMakeFiles/file_system_gui.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/file_system_gui.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/file_system_gui.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/file_system_gui.dir/flags.make

CMakeFiles/file_system_gui.dir/file_system_gui_autogen/mocs_compilation.cpp.o: CMakeFiles/file_system_gui.dir/flags.make
CMakeFiles/file_system_gui.dir/file_system_gui_autogen/mocs_compilation.cpp.o: file_system_gui_autogen/mocs_compilation.cpp
CMakeFiles/file_system_gui.dir/file_system_gui_autogen/mocs_compilation.cpp.o: CMakeFiles/file_system_gui.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jm/1st_project_2020136085/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/file_system_gui.dir/file_system_gui_autogen/mocs_compilation.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/file_system_gui.dir/file_system_gui_autogen/mocs_compilation.cpp.o -MF CMakeFiles/file_system_gui.dir/file_system_gui_autogen/mocs_compilation.cpp.o.d -o CMakeFiles/file_system_gui.dir/file_system_gui_autogen/mocs_compilation.cpp.o -c /home/jm/1st_project_2020136085/build/file_system_gui_autogen/mocs_compilation.cpp

CMakeFiles/file_system_gui.dir/file_system_gui_autogen/mocs_compilation.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/file_system_gui.dir/file_system_gui_autogen/mocs_compilation.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jm/1st_project_2020136085/build/file_system_gui_autogen/mocs_compilation.cpp > CMakeFiles/file_system_gui.dir/file_system_gui_autogen/mocs_compilation.cpp.i

CMakeFiles/file_system_gui.dir/file_system_gui_autogen/mocs_compilation.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/file_system_gui.dir/file_system_gui_autogen/mocs_compilation.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jm/1st_project_2020136085/build/file_system_gui_autogen/mocs_compilation.cpp -o CMakeFiles/file_system_gui.dir/file_system_gui_autogen/mocs_compilation.cpp.s

CMakeFiles/file_system_gui.dir/src/main.cpp.o: CMakeFiles/file_system_gui.dir/flags.make
CMakeFiles/file_system_gui.dir/src/main.cpp.o: ../src/main.cpp
CMakeFiles/file_system_gui.dir/src/main.cpp.o: CMakeFiles/file_system_gui.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jm/1st_project_2020136085/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/file_system_gui.dir/src/main.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/file_system_gui.dir/src/main.cpp.o -MF CMakeFiles/file_system_gui.dir/src/main.cpp.o.d -o CMakeFiles/file_system_gui.dir/src/main.cpp.o -c /home/jm/1st_project_2020136085/src/main.cpp

CMakeFiles/file_system_gui.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/file_system_gui.dir/src/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jm/1st_project_2020136085/src/main.cpp > CMakeFiles/file_system_gui.dir/src/main.cpp.i

CMakeFiles/file_system_gui.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/file_system_gui.dir/src/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jm/1st_project_2020136085/src/main.cpp -o CMakeFiles/file_system_gui.dir/src/main.cpp.s

CMakeFiles/file_system_gui.dir/src/commands.c.o: CMakeFiles/file_system_gui.dir/flags.make
CMakeFiles/file_system_gui.dir/src/commands.c.o: ../src/commands.c
CMakeFiles/file_system_gui.dir/src/commands.c.o: CMakeFiles/file_system_gui.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jm/1st_project_2020136085/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/file_system_gui.dir/src/commands.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/file_system_gui.dir/src/commands.c.o -MF CMakeFiles/file_system_gui.dir/src/commands.c.o.d -o CMakeFiles/file_system_gui.dir/src/commands.c.o -c /home/jm/1st_project_2020136085/src/commands.c

CMakeFiles/file_system_gui.dir/src/commands.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/file_system_gui.dir/src/commands.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/jm/1st_project_2020136085/src/commands.c > CMakeFiles/file_system_gui.dir/src/commands.c.i

CMakeFiles/file_system_gui.dir/src/commands.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/file_system_gui.dir/src/commands.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/jm/1st_project_2020136085/src/commands.c -o CMakeFiles/file_system_gui.dir/src/commands.c.s

CMakeFiles/file_system_gui.dir/src/utils.c.o: CMakeFiles/file_system_gui.dir/flags.make
CMakeFiles/file_system_gui.dir/src/utils.c.o: ../src/utils.c
CMakeFiles/file_system_gui.dir/src/utils.c.o: CMakeFiles/file_system_gui.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jm/1st_project_2020136085/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/file_system_gui.dir/src/utils.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/file_system_gui.dir/src/utils.c.o -MF CMakeFiles/file_system_gui.dir/src/utils.c.o.d -o CMakeFiles/file_system_gui.dir/src/utils.c.o -c /home/jm/1st_project_2020136085/src/utils.c

CMakeFiles/file_system_gui.dir/src/utils.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/file_system_gui.dir/src/utils.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/jm/1st_project_2020136085/src/utils.c > CMakeFiles/file_system_gui.dir/src/utils.c.i

CMakeFiles/file_system_gui.dir/src/utils.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/file_system_gui.dir/src/utils.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/jm/1st_project_2020136085/src/utils.c -o CMakeFiles/file_system_gui.dir/src/utils.c.s

CMakeFiles/file_system_gui.dir/src/mainwindow.cpp.o: CMakeFiles/file_system_gui.dir/flags.make
CMakeFiles/file_system_gui.dir/src/mainwindow.cpp.o: ../src/mainwindow.cpp
CMakeFiles/file_system_gui.dir/src/mainwindow.cpp.o: CMakeFiles/file_system_gui.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jm/1st_project_2020136085/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/file_system_gui.dir/src/mainwindow.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/file_system_gui.dir/src/mainwindow.cpp.o -MF CMakeFiles/file_system_gui.dir/src/mainwindow.cpp.o.d -o CMakeFiles/file_system_gui.dir/src/mainwindow.cpp.o -c /home/jm/1st_project_2020136085/src/mainwindow.cpp

CMakeFiles/file_system_gui.dir/src/mainwindow.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/file_system_gui.dir/src/mainwindow.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jm/1st_project_2020136085/src/mainwindow.cpp > CMakeFiles/file_system_gui.dir/src/mainwindow.cpp.i

CMakeFiles/file_system_gui.dir/src/mainwindow.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/file_system_gui.dir/src/mainwindow.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jm/1st_project_2020136085/src/mainwindow.cpp -o CMakeFiles/file_system_gui.dir/src/mainwindow.cpp.s

# Object files for target file_system_gui
file_system_gui_OBJECTS = \
"CMakeFiles/file_system_gui.dir/file_system_gui_autogen/mocs_compilation.cpp.o" \
"CMakeFiles/file_system_gui.dir/src/main.cpp.o" \
"CMakeFiles/file_system_gui.dir/src/commands.c.o" \
"CMakeFiles/file_system_gui.dir/src/utils.c.o" \
"CMakeFiles/file_system_gui.dir/src/mainwindow.cpp.o"

# External object files for target file_system_gui
file_system_gui_EXTERNAL_OBJECTS =

file_system_gui: CMakeFiles/file_system_gui.dir/file_system_gui_autogen/mocs_compilation.cpp.o
file_system_gui: CMakeFiles/file_system_gui.dir/src/main.cpp.o
file_system_gui: CMakeFiles/file_system_gui.dir/src/commands.c.o
file_system_gui: CMakeFiles/file_system_gui.dir/src/utils.c.o
file_system_gui: CMakeFiles/file_system_gui.dir/src/mainwindow.cpp.o
file_system_gui: CMakeFiles/file_system_gui.dir/build.make
file_system_gui: /usr/lib/x86_64-linux-gnu/libQt6Widgets.so.6.2.4
file_system_gui: /usr/lib/x86_64-linux-gnu/libQt6Gui.so.6.2.4
file_system_gui: /usr/lib/x86_64-linux-gnu/libQt6Core.so.6.2.4
file_system_gui: /usr/lib/x86_64-linux-gnu/libGLX.so
file_system_gui: /usr/lib/x86_64-linux-gnu/libOpenGL.so
file_system_gui: CMakeFiles/file_system_gui.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jm/1st_project_2020136085/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable file_system_gui"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/file_system_gui.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/file_system_gui.dir/build: file_system_gui
.PHONY : CMakeFiles/file_system_gui.dir/build

CMakeFiles/file_system_gui.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/file_system_gui.dir/cmake_clean.cmake
.PHONY : CMakeFiles/file_system_gui.dir/clean

CMakeFiles/file_system_gui.dir/depend:
	cd /home/jm/1st_project_2020136085/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jm/1st_project_2020136085 /home/jm/1st_project_2020136085 /home/jm/1st_project_2020136085/build /home/jm/1st_project_2020136085/build /home/jm/1st_project_2020136085/build/CMakeFiles/file_system_gui.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/file_system_gui.dir/depend
