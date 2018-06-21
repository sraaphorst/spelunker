/**
 * test_thickify.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <iostream>

#include <typeclasses/Homomorphism.h>
#include <typeclasses/Show.h>
#include <maze/DFSMazeGenerator.h>
#include <maze/Maze.h>
#include <maze/MazeTypeclasses.h>
#include <thickmaze/ThickMaze.h>
#include <thickmaze/ThickMazeTypeclasses.h>

int main(int argc, char *argv[]) {
    spelunker::maze::DFSMazeGenerator dfs(25, 20);
    spelunker::maze::Maze m = dfs.generate();
    std::cout << spelunker::typeclasses::Show<spelunker::maze::Maze>::show(m);

    spelunker::thickmaze::ThickMaze tm = spelunker::typeclasses::Homomorphism<spelunker::maze::Maze, spelunker::thickmaze::ThickMaze>::morph(m);
    std::cout << "ThickMaze has size: " << spelunker::typeclasses::Show<spelunker::types::Dimensions2D>::show(tm.getDimensions()) << std::endl;
    std::cout << spelunker::typeclasses::Show<spelunker::thickmaze::ThickMaze>::show(tm);
    return 0;
}