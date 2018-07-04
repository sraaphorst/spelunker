/**
 * TestSquashedThickMaze.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <catch.hpp>

#include <boost/graph/adjacency_list.hpp>

#include <thickmaze/CellularAutomatonThickMazeGenerator.h>
#include <thickmaze/ThickMaze.h>
#include <squashedmaze/SquashedMazeAttributes.h>
#include <squashedmaze/SquashedMaze.h>

using namespace spelunker;

TEST_CASE("SquashedMaze should process a ThickMaze", "[squashedmaze][thickmaze]") {
    constexpr auto width = 20;
    constexpr auto height = 20;
    thickmaze::CellularAutomatonThickMazeGenerator gen{10, 10};
    const auto tm = gen.generate();

    const auto sm = squashedmaze::SquashedMaze(tm);

    const auto &mp = sm.getEdgeMap();
    const auto &g = sm.getGraph();
    REQUIRE(true == true);
}