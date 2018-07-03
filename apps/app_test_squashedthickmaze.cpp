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
    constexpr auto width = 50;
    constexpr auto height = 50;
    constexpr auto numCells = width * height;

    CellularAutomatonThickMazeGenerator::settings s{};
    CellularAutomatonThickMazeGenerator gen{50, 50, s};
    ThickMaze tm = gen.generate();
    cout << typeclasses::Show<spelunker::thickmaze::ThickMaze>::show(tm) << endl;

    const auto sm = squashedmaze::SquashedMaze(tm);
    const auto &vm = sm.getVertexMap();
    const auto &em = sm.getEdgeMap();

    cout << "    Number of vertices in the squashed maze: " << setw(8) << right << vm.size()<< endl;
    cout << "       Number of cells in the original maze: " << setw(8) << right << numCells << endl;
    cout << "                    % size of squashed maze: " << setw(8) << setprecision(4) << right << ((100.0 * vm.size()) / numCells) << '%' << endl;
    cout << endl;

    const auto carvedWalls = tm.numCarvedWalls();
    cout << "       Number of edges in the squashed maze: " << setw(8) << right << em.size() << endl;
    cout << "Number of carved walls in the original maze: " << setw(8) << right << carvedWalls << endl;
    cout << "                    % size of squashed maze: " << setw(8) << setprecision(4) << right << ((100.0 * em.size()) / carvedWalls) << '%' << endl;
}