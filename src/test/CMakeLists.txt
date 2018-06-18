cmake_minimum_required(VERSION 3.1)
project(cmake_test)

# Enables testing from this directory downwards.
enable_testing(true)

# Prepare "Catch" library for other executables
set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

# Make test executable
set(TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)
add_executable(tests ${TEST_SOURCES})
target_link_libraries(tests Catch)

add_executable(section section.cpp main.cpp)
target_link_libraries(section spelunker Catch)

add_subdirectory(maze)