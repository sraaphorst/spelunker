add_executable(thickmaze_widget ThickMazeWidget.cpp MazeWidget.cpp)
set_target_properties(thickmaze_widget PROPERTIES COMPILE_FLAGS "-DMAZE_TYPE=2")
target_link_libraries(thickmaze_widget LINK_PUBLIC Qt5::Widgets Qt5::Core spelunker)

add_executable(maze_widget MazeWidget.cpp)
set_target_properties(maze_widget PROPERTIES COMPILE_FLAGS "-DMAZE_TYPE=1")
target_link_libraries(maze_widget LINK_PUBLIC Qt5::Widgets Qt5::Core spelunker)


add_executable(mazerunner mazerunner.cpp)
target_link_libraries(mazerunner
        Qt5::Widgets
        spelunker)