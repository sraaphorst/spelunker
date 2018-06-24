/**
 * TestBFSThickMaze.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include <vector>

#include <types/CommonMazeAttributes.h>
#include <types/Dimensions2D.h>
#include <thickmaze/ThickMaze.h>
#include <thickmaze/ThickMazeAttributes.h>

// We use a cellular automaton since this will almost certainly
// result in a maze that is not connected.
#include <thickmaze/CellularAutomatonThickMazeGenerator.h>

using namespace spelunker;

static constexpr auto width  = 80;
static constexpr auto height = 60;
static constexpr auto maxRk  = width * height;

// In order to simplify map lookups, we rank cells.
auto ranker = [](const types::Cell &c) {
    const auto [x, y] = c;
    return y * width + x;
};
auto unranker = [](const int rk) {
    const auto x = rk % width;
    const auto y = rk / width;
    return types::cell(x, y);
};

TEST_CASE("Ensure that the BFS starting from a cell in a ThickMaze works consistently", "[thickmaze][bfs]") {
    // Produce a maze.
    const thickmaze::CellularAutomatonThickMazeGenerator gen{width, height};
    const auto tm = gen.generate();

    // First we set up a map to record distances between cells.
    // Set identical cells to 0, and all other to -1 to start with.
    std::vector<std::vector<int>> D(maxRk, std::vector<int>(maxRk, -1));
    for (auto rk = 0; rk < maxRk; ++rk)
        D[rk][rk] = 0;

    // Now do a BFS from each cell in the tree.
    for (auto y = 0; y < height; ++y) {
        for (auto x = 0; x < width; ++x) {
            const auto c1  = types::cell(x, y);
            const auto rk1 = ranker(c1);
            const auto [start, comp, dist] = tm.performBFSFrom(c1);

            // The start position must have been properly recorded.
            REQUIRE(start == c1);

            // If c1 is a wall, ensure comp only has one cell and only one
            // distance size.
            if (tm.cellIs(c1) == thickmaze::CellType::WALL) {
                REQUIRE(comp.size() == 1);
                REQUIRE(dist.size() == 1);
                REQUIRE(dist[0].size() == 1);
                REQUIRE(*(dist[0].begin()) == start);
            }

            // Mark the distances, making sure existing ones match up.
            for (auto d = 0; d < dist.size(); ++d) {
                // Retrieve the set of cells at distance d from the start.
                const auto dv = dist[d];

                // Now, for each cell c at this distance, perform the following:
                // 1. If D[c1][c] exists, make sure it is d.
                //    Make sure that D[c][c1] is also d.
                // 2. If D[c1][c] does not exist, make sure D[c][c1] also does not exist.
                //    Set both to d.
                for (const auto c2: dv) {
                    const auto rk = ranker(c2);

                    if (D[rk1][rk] != -1) {
                        // Both distances
                        REQUIRE(D[rk1][rk] == d);
                        REQUIRE(D[rk][rk1] == d);
                    } else {
                        REQUIRE(D[rk][rk1] == -1);
                        D[rk1][rk] = d;
                        D[rk][rk1] = d;
                    }
                }
            }
        }
    }
}
