cmake_minimum_required(VERSION 2.8)

message(STATUS "Creating temp-gen application")

include_directories(${PROJECT_SOURCE_DIR}/include)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin-gen)

project(tempgen)

add_executable(tempgen tempgen.cpp)

target_link_libraries (tempgen LINK_PUBLIC SGAL)
target_include_directories (tempgen PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/lib)