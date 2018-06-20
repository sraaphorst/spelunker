# CMakeLists.txt
#
# By Sebastian Raaphorst, 2018.

add_executable(test_dimensions2d TestDimensions2D.cpp ${TEST_SOURCES})
target_link_libraries(test_dimensions2d Catch ${Boost_SERIALIZATION_LIBRARY} spelunker)

add_executable(test_symmetry TestSymmetry.cpp ${TEST_SOURCES})
target_link_libraries(test_symmetry Catch ${Boost_SERIALIZATION_LIBRARY} spelunker)