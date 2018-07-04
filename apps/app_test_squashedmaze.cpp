/**
 * app_test_squishedmaze.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <iostream>
#include <iomanip>
#include <cmath>

#include <boost/graph/adjacency_list.hpp>

#include <maze/DFSMazeGenerator.h>
#include <maze/Maze.h>
#include <maze/MazeTypeclasses.h>
#include <squashedmaze/SquashedMazeAttributes.h>
#include <squashedmaze/SquashedMaze.h>
#include <typeclasses/Show.h>

using namespace spelunker;
using namespace std;

int main() {
    constexpr auto width = 15;
    constexpr auto height = 15;
    constexpr auto numCells = width * height;

    const maze::DFSMazeGenerator dfs{width, height};
    const auto m = dfs.generate().braidAll();
    cout << typeclasses::Show<maze::Maze>::show(m) << endl;

    const auto sm = squashedmaze::SquashedMaze(m);
    const auto &vm = sm.getVertexMap();
    const auto &em = sm.getEdgeMap();

    cout << "    Number of vertices in the squashed maze: " << setw(8) << right << vm.size()<< endl;
    cout << "       Number of cells in the original maze: " << setw(8) << right << numCells << endl;
    cout << "                    % size of squashed maze: " << setw(8) << setprecision(4) << right << ((100.0 * vm.size()) / numCells) << '%' << endl;
    cout << endl;

    const auto carvedWalls = m.numCarvedWalls();
    cout << "       Number of edges in the squashed maze: " << setw(8) << right << em.size() << endl;
    cout << "Number of carved walls in the original maze: " << setw(8) << right << carvedWalls << endl;
    cout << "                    % size of squashed maze: " << setw(8) << setprecision(4) << right << ((100.0 * em.size()) / carvedWalls) << '%' << endl;
}