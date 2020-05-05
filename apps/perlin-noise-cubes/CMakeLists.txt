cmake_minimum_required(VERSION 2.8)

message(STATUS "Creating perlin noise cubes application")

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/apps/include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/apps/perlin-noise-cubes)

project(perlin-noise-cubes)

add_executable(perlin-noise-cubes main.cpp)

target_link_libraries (perlin-noise-cubes LINK_PUBLIC SGAL)
target_include_directories (perlin-noise-cubes PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/lib)

add_custom_command(TARGET perlin-noise-cubes POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/../../lib/libSGAL.dll ${PROJECT_SOURCE_DIR}/libSGAL.dll
)