/**
 * test_unicursal.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <iostream>
using namespace std;

#include "maze/DFSMazeGenerator.h"
#include "maze/Maze.h"
#include "typeclasses/Show.h"

int main(int argc, char *argv[]) {
    spelunker::maze::DFSMazeGenerator dfs(10, 8);
    spelunker::maze::Maze m = dfs.generate();
    std::cout << spelunker::typeclasses::Show<spelunker::maze::Maze>::show(m);

    spelunker::maze::Maze mu = m.makeUnicursal();
    std::cout << spelunker::typeclasses::Show<spelunker::maze::Maze>::show(mu);

    spelunker::maze::Maze mu2 = mu.makeUnicursal();
    std::cout << spelunker::typeclasses::Show<spelunker::maze::Maze>::show(mu2);
}