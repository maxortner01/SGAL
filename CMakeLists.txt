cmake_minimum_required(VERSION 2.8)

option(COMPILE_CLIENT "Compile the client program" ON)
option(COMPILE_TEMPGEN "Compile the program to generate templates" ON)

project(SGAL)

add_subdirectory(extern)
add_subdirectory(src)

if (COMPILE_CLIENT)
    add_subdirectory(client)
endif()

if (COMPILE_TEMPGEN)
    add_subdirectory(bin-gen)
    #file(COPY ${PROJECT_SOURCE_DIR}/lib/libSGAL.dll DESTINATION ${PROJECT_SOURCE_DIR}/bin-gen)
endif()

