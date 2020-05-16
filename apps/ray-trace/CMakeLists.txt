cmake_minimum_required(VERSION 2.8)

message(STATUS "Creating ray trace application")

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/apps/include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/apps/ray-trace)

project(ray-trace)

add_executable(ray-trace main.cpp)

target_link_libraries (ray-trace LINK_PUBLIC SGAL)
target_include_directories (ray-trace PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/lib)

add_custom_command(TARGET ray-trace POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/../../lib/libSGAL.dll ${PROJECT_SOURCE_DIR}/libSGAL.dll
)