/**
 * test_thickify.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <iostream>
using namespace std;

#include "DFSMazeGenerator.h"
#include "ThickMaze.h"
#include "Maze.h"
#include "Homomorphism.h"
#include "Show.h"

int main(int argc, char *argv[]) {
    spelunker::maze::DFSMazeGenerator dfs(20, 15);
    spelunker::maze::Maze m = dfs.generate();
    std::cout << spelunker::typeclasses::Show<spelunker::maze::Maze>::show(m);

    spelunker::thickmaze::ThickMaze tm = spelunker::typeclasses::Homomorphism<spelunker::maze::Maze, spelunker::thickmaze::ThickMaze>::morph(m);
    std::cout << spelunker::typeclasses::Show<spelunker::thickmaze::ThickMaze>::show(tm);
}