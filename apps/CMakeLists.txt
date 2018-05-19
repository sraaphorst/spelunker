# Find includes in corresponding build directories.
set(CMAKE_PREFIX_PATH "/Applications/Qt/5.10.1/clang_64")
set(CMAKE_INCLUDE_CURRENT_DIR  ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

find_package(Qt5Widgets REQUIRED)

add_executable(mazerunner mazerunner.cpp)
target_link_libraries(mazerunner
        Qt5::Widgets
        spelunker)

add_executable(kruskal kruskal.cpp)
target_link_libraries(kruskal LINK_PUBLIC spelunker)