cmake_minimum_required(VERSION 3.19)

project(SearchAlgo LANGUAGES CXX)

add_library(commom STATIC Edge.cpp Vertex.cpp TreeSearch.cpp)

add_executable(bfs main.cpp)
target_link_libraries(bfs commom)
