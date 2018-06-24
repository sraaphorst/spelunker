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

    // There is an incredibly small chance that this could fail.
    REQUIRE(m1 != m2);
    REQUIRE(m1 == ml1);
    REQUIRE(m2 == ml2);
    REQUIRE(ml1 != ml2);
}

// We can say much less here about Maze than ThickMaze due to
// the possibility of cells being cut off by braiding.
TEST_CASE("Maze neighbours should obey expected rules") {
    constexpr auto width = 50;
    constexpr auto height = 40;
    const maze::DFSMazeGenerator dfs{width, height};
    const auto m = dfs.generate();

    SECTION("Every cell has beween 0 and 4 neighbours") {
        for (auto x = 0; x < m.getWidth(); ++x)
            for (auto y = 0; y < m.getHeight(); ++y) {
                const types::Cell c = types::cell(x, y);
                REQUIRE(m.neighbours(c).size() >= 0);
                REQUIRE(m.neighbours(c).size() <= 4);
            }
    }
    SECTION("Every cell has between 0 and 4 walls") {
        for (auto x = 0; x < m.getWidth(); ++x)
            for (auto y = 0; y < m.getHeight(); ++y) {
                const types::Cell c = types::cell(x, y);
                REQUIRE(m.numCellWalls(c) >= 0);
                REQUIRE(m.numCellWalls(c) <= 4);
            }
    }
    SECTION("For a given cell, the number of neighbours and the number of walls should sum to four") {
        for (auto x = 0; x < m.getWidth(); ++x)
            for (auto y = 0; y < m.getHeight(); ++y) {
                const types::Cell c = types::cell(x, y);
                const auto numNbrs = m.neighbours(c).size();
                const auto numWalls = m.numCellWalls(c);
                if (numNbrs + numWalls != 4)
                    REQUIRE(numNbrs + numWalls == 4);
            }
    }
}
