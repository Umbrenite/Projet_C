# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

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
CMAKE_SOURCE_DIR = /home/arthur/Projet

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/arthur/Projet/build

# Include any dependencies generated for this target.
include CMakeFiles/Projet.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Projet.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Projet.dir/flags.make

CMakeFiles/Projet.dir/main.c.o: CMakeFiles/Projet.dir/flags.make
CMakeFiles/Projet.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/arthur/Projet/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/Projet.dir/main.c.o"
	/usr/bin/x86_64-linux-gnu-gcc-10 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Projet.dir/main.c.o -c /home/arthur/Projet/main.c

CMakeFiles/Projet.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Projet.dir/main.c.i"
	/usr/bin/x86_64-linux-gnu-gcc-10 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/arthur/Projet/main.c > CMakeFiles/Projet.dir/main.c.i

CMakeFiles/Projet.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Projet.dir/main.c.s"
	/usr/bin/x86_64-linux-gnu-gcc-10 $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/arthur/Projet/main.c -o CMakeFiles/Projet.dir/main.c.s

# Object files for target Projet
Projet_OBJECTS = \
"CMakeFiles/Projet.dir/main.c.o"

# External object files for target Projet
Projet_EXTERNAL_OBJECTS =

Projet: CMakeFiles/Projet.dir/main.c.o
Projet: CMakeFiles/Projet.dir/build.make
Projet: CMakeFiles/Projet.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/arthur/Projet/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable Projet"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Projet.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Projet.dir/build: Projet

.PHONY : CMakeFiles/Projet.dir/build

CMakeFiles/Projet.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Projet.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Projet.dir/clean

CMakeFiles/Projet.dir/depend:
	cd /home/arthur/Projet/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/arthur/Projet /home/arthur/Projet /home/arthur/Projet/build /home/arthur/Projet/build /home/arthur/Projet/build/CMakeFiles/Projet.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Projet.dir/depend

