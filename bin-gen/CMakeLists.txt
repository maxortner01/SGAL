cmake_minimum_required(VERSION 2.8)

message(STATUS "Creating temp-gen application")

include_directories(${PROJECT_SOURCE_DIR}/include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin-gen)

project(tempgen)

add_executable(tempgen tempgen.cpp)

target_link_libraries (tempgen LINK_PUBLIC SGAL)
target_include_directories (tempgen PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/lib)

add_custom_command(TARGET tempgen POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/../lib/libSGAL.dll ${PROJECT_SOURCE_DIR}/libSGAL.dll
)