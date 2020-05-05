cmake_minimum_required(VERSION 2.8)

option(COMPILE_APPS "Compile the applications" ON)
option(COMPILE_TEMPGEN "Compile the program to generate templates" ON)

project(SGAL)

add_subdirectory(extern)
add_subdirectory(src)

if (COMPILE_APPS)
    add_subdirectory(apps)
endif()

if (COMPILE_TEMPGEN)
    add_subdirectory(bin-gen)
endif()

