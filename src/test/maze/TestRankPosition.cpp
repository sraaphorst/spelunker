/**
 * RankPositionTest.cpp
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Tests the @see{spelunker::maze::Maze::rankPositionS} function used by Mazes to
 * map positions into WallIDs.
 */

#include <catch.hpp>

#include <set>

#include <types/CommonMazeAttributes.h>
#include <types/Dimensions2D.h>
#include <types/Direction.h>
#include <maze/Maze.h>
#include <maze/MazeAttributes.h>

using namespace spelunker;

TEST_CASE("Ranking walls for Maze representations", "[maze][rank]") {
    constexpr auto width = 50;
    constexpr auto height = 40;
    const types::Dimensions2D dim{width, height};

    std::set<int> ranks;
    const auto numwalls = maze::calculateNumWalls(dim);

    SECTION("Ranking walls from the position on either side of the wall yields the same rank") {
        for (auto x = 0; x < width; ++x)
            for (auto y = 0; y < height; ++y) {
                for (auto dir: types::directions()) {
                    const auto r = maze::Maze::rankPositionS(dim, x, y, dir);
                    if (r != -1) ranks.insert(r);
                }
                if (y > 0 && y < height - 1)
                    REQUIRE(maze::Maze::rankPositionS(dim, x, y, types::Direction::NORTH) ==
                            maze::Maze::rankPositionS(dim, x, y - 1, types::Direction::SOUTH));
                if (x > 0 && x < width - 1)
                    REQUIRE(maze::Maze::rankPositionS(dim, x - 1, y, types::Direction::EAST) ==
                            maze::Maze::rankPositionS(dim, x, y, types::Direction::WEST));
            }
    }

    SECTION("For any wall rank, there is a position that ranks to it") {
        for (auto x = 0; x < width; ++x)
            for (auto y = 0; y < height; ++y) {
                for (auto dir: types::directions()) {
                    const auto r = maze::Maze::rankPositionS(dim, x, y, dir);
                    if (r != -1) ranks.insert(r);
                }
            }

        for (auto i = 0; i < numwalls; ++i)
            REQUIRE(ranks.find(i) != ranks.end());

    }
}