/**
 * TestUnrankWallMap.cpp
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Tests the @see{spelunker::maze::MazeGenerator::createUnrankWallMapS} function that
 * unranks WallIds into pairs of positions.
 */

#include <catch.hpp>

#include <types/Dimensions2D.h>
#include <maze/Maze.h>
#include <maze/MazeGenerator.h>

using namespace spelunker;

TEST_CASE("Unranking map from wall ranks to pairs of adjacent positions", "[maze][unrank]") {
    constexpr auto width = 50;
    constexpr auto height = 40;
    const types::Dimensions2D dim{width, height};

    const auto m = maze::MazeGenerator::createUnrankWallMapS(dim);

    SECTION("The unranking wall map produces two positions, which both rank back to the wall id") {
        for (const auto &kv : m) {
            // kv is std::map<WallID, std::pair<types::Position, types::Position> >
            const auto &[rk, ps] = kv;
            const auto &[p1, p2] = ps;

            const auto &[c1, d1] = p1;
            const auto[x1, y1] = c1;
            REQUIRE(maze::Maze::rankPositionS(dim, x1, y1, d1) == rk);

            const auto &[c2, d2] = p2;
            const auto[x2, y2] = c2;
            REQUIRE(maze::Maze::rankPositionS(dim, x2, y2, d2) == rk);
        }
    }
}