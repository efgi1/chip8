# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.24)


project ("chip8")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(${CMAKE_BINARY_DIR}/conanbuildinfo_multi.cmake)
conan_basic_setup()

# Include sub-projects.
add_subdirectory ("src")

foreach(_LIB ${CONAN_LIBS_RELEASE})
	target_link_libraries(${PROJECT_NAME} optimized ${_LIB})
endforeach()
foreach(_LIB ${CONAN_LIBS_DEBUG})
	target_link_libraries(${PROJECT_NAME} debug ${_LIB})
endforeach()

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})