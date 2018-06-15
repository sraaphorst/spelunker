# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

set (CMAKE_PREFIX_PATH "/Applications/Qt-5.9.4/5.9.4/clang_64" CACHE PATH "")
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

add_executable(test_thickmaze_widget ThickMazeWidget.cpp MazeWidget.cpp ThickMazeWidgetTest.cpp)
target_link_libraries(test_thickmaze_widget LINK_PUBLIC Qt5::Widgets spelunker)

add_executable(test_maze_widget MazeWidget.cpp MazeWidgetTest.cpp)
target_link_libraries(test_maze_widget LINK_PUBLIC Qt5::Widgets spelunker)
