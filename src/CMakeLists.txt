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

# Check if Python is installed on the machine
find_package(Python COMPONENTS Interpreter)
if (Python_FOUND)
    message(STATUS "Python Version ${Python_VERSION} installed")

    # Call the script for generating primitives
    add_custom_target(
    primitive-gen ALL
    COMMAND ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/../scripts/genprims.py ${CMAKE_CURRENT_SOURCE_DIR}
    BYPRODUCTS Primitives.h
    COMMENT "Generating primitive files"
    )
    
    # Call the script for generating shaders
    add_custom_target(
    shader-gen ALL
    COMMAND ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/../scripts/genshaders.py ${CMAKE_CURRENT_SOURCE_DIR}
    BYPRODUCTS Shaders.h
    COMMENT "Generating shaders header/source files"
    )

    add_dependencies(SGAL primitive-gen)
    add_dependencies(SGAL shader-gen)

else()
    add_definitions(-DSG_NO_PRIMITIVES)
    message(WARNING "Warning: Python not found, cannot generate primitive header files")
endif()

if (WIN32)
    if (MSVC)
        message(STATUS "Making SGAL library for Windows MSVC.")
        target_link_libraries(SGAL opengl32.lib)
    else()
        message(STATUS "Making SGAL library for Windows GNU.")
        target_link_libraries(SGAL opengl32)
    endif()
endif()