# CMakeList.txt : CMake project for chip8, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (chip8 "chip8.cpp" "chip8.h"   "VirtualChip8.cpp" "VirtualChip8.h"  "Display.cpp" "Display.h" "Shader.cpp" "Shader.h" "Input.cpp" "Input.h")

include_directories("irrKlang\\include")
target_link_libraries(chip8 "${CMAKE_CURRENT_SOURCE_DIR}\\irrKlang\\lib\\irrKlang.lib")

