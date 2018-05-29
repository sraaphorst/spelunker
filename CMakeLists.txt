cmake_minimum_required(VERSION 3.10)
project(spelunker
  VERSION 0.1.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

include_directories("${PROJECT_SOURCE_DIR}/src")
add_subdirectory(src)
add_subdirectory(apps)



