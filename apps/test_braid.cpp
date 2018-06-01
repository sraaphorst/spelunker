/**
 * test_braid.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <iostream>
using namespace std;

#include "maze/HuntAndKillMazeGenerator.h"
#include "maze/Maze.h"
#include "typeclasses/Show.h"

int main(int argc, char *argv[]) {
    spelunker::maze::HuntAndKillMazeGenerator gen(50, 40);
    spelunker::maze::Maze m = gen.generate().braid();
    std::cout << spelunker::typeclasses::Show<spelunker::maze::Maze>::show(m);
    return 0;
}