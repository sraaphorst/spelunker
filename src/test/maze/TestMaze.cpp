/**
 * TestMaze.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <sstream>

#include <maze/DFSMazeGenerator.h>
#include <maze/Maze.h>

using namespace spelunker;

TEST_CASE("Maze should be able to serialize and deserialize", "[maze][serialization]") {
    constexpr auto width = 50;
    constexpr auto height = 40;
    const maze::DFSMazeGenerator dfs{width, height};

    const auto m1 = dfs.generate();
    const auto m2 = dfs.generate();

    std::stringstream ss1;
    std::stringstream ss2;

    m1.save(ss1);
    m2.save(ss2);

    const auto ml1 = maze::Maze::load(ss1);
    const auto ml2 = maze::Maze::load(ss2);

    // It is possible but vanishingly unlikely that this will fail.
    REQUIRE(m1 != m2);
    REQUIRE(m1 == ml1);
    REQUIRE(m2 == ml2);
    REQUIRE(ml1 != ml2);
}