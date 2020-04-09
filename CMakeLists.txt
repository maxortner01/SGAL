cmake_minimum_required(VERSION 2.8)

option(COMPILE_CLIENT "Compile the client program" ON)

project(SGAL)

add_subdirectory(extern)
add_subdirectory(src)

if (COMPILE_CLIENT)
    add_subdirectory(client)
endif()