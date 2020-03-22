cmake_minimum_required(VERSION 2.8)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ../lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ../lib)

project(new-graphics)

add_definitions(-DGLEW_STATIC)

link_directories(${CMAKE_SOURCE_DIR}/extern/glfw-3.3.2/lib)
include_directories(include extern/glew-2.1.0/include extern/glfw-3.3.2/include extern/tinyobj/include)

file(GLOB SOURCES "src/*.cpp")

add_library(new-graphics SHARED ${SOURCES} "extern/glew-2.1.0/src/glew.c")

target_link_libraries(new-graphics glfw3 opengl32)
