cmake_minimum_required(VERSION 2.8)

message(STATUS "Creating marching cubes application")

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/apps/include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/apps/marching-cubes)

project(marching-cubes)

add_executable(marching-cubes main.cpp)

target_link_libraries (marching-cubes LINK_PUBLIC SGAL)
target_include_directories (marching-cubes PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/lib)

add_custom_command(TARGET marching-cubes POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/../../lib/libSGAL.dll ${PROJECT_SOURCE_DIR}/libSGAL.dll
)