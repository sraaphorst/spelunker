/**
 * test_thickify.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <iostream>

#include "maze/DFSMazeGenerator.h"
#include "thickmaze/ThickMaze.h"
#include "maze/Maze.h"
#include "typeclasses/Homomorphism.h"
#include "typeclasses/Show.h"

int main(int argc, char *argv[]) {
    spelunker::maze::DFSMazeGenerator dfs(25, 20);
    spelunker::maze::Maze m = dfs.generate();
    std::cout << spelunker::typeclasses::Show<spelunker::maze::Maze>::show(m);

    spelunker::thickmaze::ThickMaze tm = spelunker::typeclasses::Homomorphism<spelunker::maze::Maze, spelunker::thickmaze::ThickMaze>::morph(m);
    std::cout << spelunker::typeclasses::Show<spelunker::thickmaze::ThickMaze>::show(tm);
    return 0;
}