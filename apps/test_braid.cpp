/**
 * test_braid.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <iostream>
using namespace std;

#include "maze/WilsonMazeGenerator.h"
#include "maze/Maze.h"
#include "typeclasses/Show.h"

int main(int argc, char *argv[]) {
    spelunker::maze::WilsonMazeGenerator dfs(50, 40);
    spelunker::maze::Maze m = dfs.generate();
    std::cout << spelunker::typeclasses::Show<spelunker::maze::Maze>::show(m);

    spelunker::maze::Maze mb = m.braid();
    std::cout << spelunker::typeclasses::Show<spelunker::maze::Maze>::show(mb);

    return 0;
}