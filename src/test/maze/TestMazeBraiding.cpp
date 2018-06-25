/**
 * TestMazeBraiding.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include <maze/DFSMazeGenerator.h>
#include <maze/Maze.h>

using namespace spelunker;

TEST_CASE("Maze should be able to serialize and deserialize", "[maze][braiding]") {
    constexpr auto width = 50;
    constexpr auto height = 40;
    const maze::DFSMazeGenerator dfs{width, height};

    const maze::Maze m  = dfs.generate();
    const maze::Maze mb = m.braidAll();
    REQUIRE(m != mb);

    const maze::Maze mb2 = mb.braidAll();
    REQUIRE(mb == mb2);
}