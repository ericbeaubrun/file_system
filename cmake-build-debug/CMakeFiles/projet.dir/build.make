# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

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
CMAKE_COMMAND = /cygdrive/c/Users/menwa/AppData/Local/JetBrains/CLion2023.3/cygwin_cmake/bin/cmake.exe

# The command to remove a file.
RM = /cygdrive/c/Users/menwa/AppData/Local/JetBrains/CLion2023.3/cygwin_cmake/bin/cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /cygdrive/c/JetBrains/Clion/file_system

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /cygdrive/c/JetBrains/Clion/file_system/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/projet.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/projet.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/projet.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/projet.dir/flags.make

CMakeFiles/projet.dir/main.c.o: CMakeFiles/projet.dir/flags.make
CMakeFiles/projet.dir/main.c.o: /cygdrive/c/JetBrains/Clion/file_system/main.c
CMakeFiles/projet.dir/main.c.o: CMakeFiles/projet.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/cygdrive/c/JetBrains/Clion/file_system/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/projet.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/projet.dir/main.c.o -MF CMakeFiles/projet.dir/main.c.o.d -o CMakeFiles/projet.dir/main.c.o -c /cygdrive/c/JetBrains/Clion/file_system/main.c

CMakeFiles/projet.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/projet.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /cygdrive/c/JetBrains/Clion/file_system/main.c > CMakeFiles/projet.dir/main.c.i

CMakeFiles/projet.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/projet.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /cygdrive/c/JetBrains/Clion/file_system/main.c -o CMakeFiles/projet.dir/main.c.s

CMakeFiles/projet.dir/src/library.c.o: CMakeFiles/projet.dir/flags.make
CMakeFiles/projet.dir/src/library.c.o: /cygdrive/c/JetBrains/Clion/file_system/src/library.c
CMakeFiles/projet.dir/src/library.c.o: CMakeFiles/projet.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/cygdrive/c/JetBrains/Clion/file_system/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/projet.dir/src/library.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/projet.dir/src/library.c.o -MF CMakeFiles/projet.dir/src/library.c.o.d -o CMakeFiles/projet.dir/src/library.c.o -c /cygdrive/c/JetBrains/Clion/file_system/src/library.c

CMakeFiles/projet.dir/src/library.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/projet.dir/src/library.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /cygdrive/c/JetBrains/Clion/file_system/src/library.c > CMakeFiles/projet.dir/src/library.c.i

CMakeFiles/projet.dir/src/library.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/projet.dir/src/library.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /cygdrive/c/JetBrains/Clion/file_system/src/library.c -o CMakeFiles/projet.dir/src/library.c.s

CMakeFiles/projet.dir/src/partition.c.o: CMakeFiles/projet.dir/flags.make
CMakeFiles/projet.dir/src/partition.c.o: /cygdrive/c/JetBrains/Clion/file_system/src/partition.c
CMakeFiles/projet.dir/src/partition.c.o: CMakeFiles/projet.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/cygdrive/c/JetBrains/Clion/file_system/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/projet.dir/src/partition.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/projet.dir/src/partition.c.o -MF CMakeFiles/projet.dir/src/partition.c.o.d -o CMakeFiles/projet.dir/src/partition.c.o -c /cygdrive/c/JetBrains/Clion/file_system/src/partition.c

CMakeFiles/projet.dir/src/partition.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing C source to CMakeFiles/projet.dir/src/partition.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /cygdrive/c/JetBrains/Clion/file_system/src/partition.c > CMakeFiles/projet.dir/src/partition.c.i

CMakeFiles/projet.dir/src/partition.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling C source to assembly CMakeFiles/projet.dir/src/partition.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /cygdrive/c/JetBrains/Clion/file_system/src/partition.c -o CMakeFiles/projet.dir/src/partition.c.s

# Object files for target projet
projet_OBJECTS = \
"CMakeFiles/projet.dir/main.c.o" \
"CMakeFiles/projet.dir/src/library.c.o" \
"CMakeFiles/projet.dir/src/partition.c.o"

# External object files for target projet
projet_EXTERNAL_OBJECTS =

projet.exe: CMakeFiles/projet.dir/main.c.o
projet.exe: CMakeFiles/projet.dir/src/library.c.o
projet.exe: CMakeFiles/projet.dir/src/partition.c.o
projet.exe: CMakeFiles/projet.dir/build.make
projet.exe: CMakeFiles/projet.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/cygdrive/c/JetBrains/Clion/file_system/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking C executable projet.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/projet.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/projet.dir/build: projet.exe
.PHONY : CMakeFiles/projet.dir/build

CMakeFiles/projet.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/projet.dir/cmake_clean.cmake
.PHONY : CMakeFiles/projet.dir/clean

CMakeFiles/projet.dir/depend:
	cd /cygdrive/c/JetBrains/Clion/file_system/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /cygdrive/c/JetBrains/Clion/file_system /cygdrive/c/JetBrains/Clion/file_system /cygdrive/c/JetBrains/Clion/file_system/cmake-build-debug /cygdrive/c/JetBrains/Clion/file_system/cmake-build-debug /cygdrive/c/JetBrains/Clion/file_system/cmake-build-debug/CMakeFiles/projet.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/projet.dir/depend

