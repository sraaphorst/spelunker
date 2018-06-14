/**
 * test_isomorphism.cpp
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Test the isomorphism between Maze and GraphMaze.
 */

#include <iostream>

#include "typeclasses/Homomorphism.h"
#include "typeclasses/Show.h"
#include "maze/DFSMazeGenerator.h"
#include "maze/Maze.h"
#include "maze/MazeTypeclasses.h"
#include "graphmaze/GraphMaze.h"
#include "graphmaze/GraphMazeTypeclasses.h"
#include "thickmaze/ThickMaze.h"
#include "thickmaze/ThickMazeTypeclasses.h"

int main(int argc, char *argv[]) {
    spelunker::maze::DFSMazeGenerator gen(50,40);
    spelunker::maze::Maze m = gen.generate();
    std::cout << spelunker::typeclasses::Show<spelunker::maze::Maze>::show(m);
    std::cout << std::endl;

    spelunker::graphmaze::GraphMaze gm = spelunker::typeclasses::Homomorphism<spelunker::maze::Maze, spelunker::graphmaze::GraphMaze>::morph(m);
    std::cout << spelunker::typeclasses::Show<spelunker::graphmaze::GraphMaze>::show(gm);
    std::cout << std::endl;

    spelunker::maze::Maze m2 = spelunker::typeclasses::Homomorphism<spelunker::graphmaze::GraphMaze, spelunker::maze::Maze>::morph(gm);
    std::cout << spelunker::typeclasses::Show<spelunker::maze::Maze>::show(m2);
    std::cout << std::endl;

    // We also test the composition homomorphism
    spelunker::thickmaze::ThickMaze tm =
            spelunker::typeclasses::CompositionHomomorphism<spelunker::graphmaze::GraphMaze,
                                                            spelunker::maze::Maze,
                                                            spelunker::thickmaze::ThickMaze>::morph(gm);
    std::cout << spelunker::typeclasses::Show<spelunker::thickmaze::ThickMaze>::show(tm);
    return 0;
}