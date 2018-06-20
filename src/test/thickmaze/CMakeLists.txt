# CMakeLists.txt
#
# By Sebastian Raaphorst, 2018.

add_executable(test_thickmaze_symmetries TestThickMazeSymmetries.cpp ${TEST_SOURCES})
target_link_libraries(test_thickmaze_symmetries Catch spelunker)