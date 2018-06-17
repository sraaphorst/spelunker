/**
 * test_braid.cpp
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Test the braid function of a Maze.
 */

#include <iostream>
using namespace std;

#include "maze/DFSMazeGenerator.h"
#include "maze/Maze.h"
#include "maze/MazeTypeclasses.h"
#include "typeclasses/Show.h"

int main(int argc, char *argv[]) {
    spelunker::maze::DFSMazeGenerator gen(40, 30);
    spelunker::maze::Maze m = gen.generate();
    std::cout << spelunker::typeclasses::Show<spelunker::maze::Maze>::show(m);

    spelunker::maze::Maze mb = m.braidAll();
    std::cout << spelunker::typeclasses::Show<spelunker::maze::Maze>::show(mb);
    return 0;
}