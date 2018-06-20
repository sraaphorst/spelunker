# CMakeLists.txt
#
# By Sebastian Raaphorst, 2018.

add_executable(test_maze_symmetries TestMazeSymmetries.cpp ${TEST_SOURCES})
target_link_libraries(test_maze_symmetries Catch spelunker)

add_executable(test_rank_position TestRankPosition.cpp ${TEST_SOURCES})
target_link_libraries(test_rank_position Catch spelunker)

add_executable(test_unrank_wall_map TestUnrankWallMap.cpp ${TEST_SOURCES})
target_link_libraries(test_unrank_wall_map Catch spelunker)