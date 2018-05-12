cmake_minimum_required(VERSION 3.10)
project(spelunker)
set(CMAKE_CXX_STANDARD 17)

# Find includes in corresponding build directories.
set(CMAKE_PREFIX_PATH "/Applications/Qt/5.10.1/clang_64")
set(CMAKE_INCLUDE_CURRENT_DIR  ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

include_directories("${PROJECT_SOURCE_DIR}/src")
add_subdirectory(src)

find_package(Qt5Widgets REQUIRED)

add_executable(mazerunner mazerunner.cpp)
target_link_libraries(mazerunner Qt5::Widgets)