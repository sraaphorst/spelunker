# Find includes in corresponding build directories.
set(CMAKE_INCLUDE_CURRENT_DIR  ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets REQUIRED)

add_executable(mazerunner mazerunner.cpp)
target_link_libraries(mazerunner
        Qt5::Widgets
        spelunker)

add_executable(kruskal kruskal.cpp Executor.h)
target_link_libraries(kruskal LINK_PUBLIC spelunker)

add_executable(dfs dfs.cpp Executor.h)
target_link_libraries(dfs LINK_PUBLIC spelunker)