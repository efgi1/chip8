# CMakeList.txt : CMake project for chip8, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (chip8 "chip8.cpp" "chip8.h"   "VirtualChip8.cpp" "VirtualChip8.h" )

# TODO: Add tests and install targets if needed.
