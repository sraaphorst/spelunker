/**
 * app_test_squishedthickmaze.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <iostream>

#include <boost/graph/adjacency_list.hpp>

#include <typeclasses/Show.h>
#include <thickmaze/CellularAutomatonThickMazeGenerator.h>
#include <thickmaze/ThickMaze.h>
#include <thickmaze/ThickMazeTypeclasses.h>
#include <squashedmaze/SquashedMazeAttributes.h>
#include <squashedmaze/SquashedMaze.h>
#include <typeclasses/Show.h>

using namespace spelunker;
using namespace std;

using namespace spelunker::thickmaze;

int main(int argc, char *argv[]) {
    CellularAutomatonThickMazeGenerator::settings s{};
    CellularAutomatonThickMazeGenerator gen{30, 30, s};
    ThickMaze tm = gen.generate();
    std::cout << spelunker::typeclasses::Show<spelunker::thickmaze::ThickMaze>::show(tm);

    const auto sm = squashedmaze::SquashedMaze(tm);
    const auto &mp = sm.getMap();
    const auto &g  = sm.getGraph();
}