﻿# CMakeList.txt : CMake project for chip8, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (chip8 "chip8.cpp" "chip8.h"   "VirtualChip8.cpp" "VirtualChip8.h" "glad.c"  "Display.cpp" "Display.h" "Shader.cpp" "Shader.h" "Input.cpp" "Input.h")
find_package(OpenGL REQUIRED)
include_directories("C:\\glad\\include" "C:\\OpenGL\\include" "C:\\irrKlang\\include")
target_link_libraries(chip8 "C:\\OpenGL\\lib\\glfw3.lib" "C:\\irrKlang\\lib\\irrKlang.lib")

