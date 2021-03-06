cmake_minimum_required(VERSION 3.19)

project(SearchAlgo LANGUAGES CXX)

add_library(commom STATIC Edge.cpp Vertex.cpp TreeSearch.cpp)

add_executable(program main.cpp Problem.cpp)
target_link_libraries(program commom)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT program)
