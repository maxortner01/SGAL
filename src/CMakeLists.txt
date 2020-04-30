cmake_minimum_required(VERSION 2.8)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

include_directories(
    ${PROJECT_SOURCE_DIR}/include 
    ${PROJECT_SOURCE_DIR}/extern/glew/include 
    ${PROJECT_SOURCE_DIR}/extern/tinyobj/include
    ${PROJECT_SOURCE_DIR}/extern/stb/include)

add_definitions(-DGLEW_STATIC)
add_definitions(-DSGAL_BUILD)
add_definitions(-DSGAL_SHARED)

set(SOURCES ${PROJECT_SOURCE_DIR}/src/Graphics.cpp
            ${PROJECT_SOURCE_DIR}/src/Utility.cpp)

add_library(SGAL SHARED ${SOURCES} "${PROJECT_SOURCE_DIR}/extern/glew/src/glew.c")

if (WIN32)
    if (MSVC)
        message(STATUS "Making SGAL library for Windows MSVC.")
        target_link_libraries(SGAL opengl32.lib)
    else()
        message(STATUS "Making SGAL library for Windows GNU.")
        target_link_libraries(SGAL opengl32)
    endif()
endif()