/**
 * test_isomorphism.cpp
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Test the isomorphism between Maze and GraphMaze.
 */

#include <iostream>

#include <typeclasses/Homomorphism.h>
#include <typeclasses/Show.h>
#include <maze/DFSMazeGenerator.h>
#include <maze/Maze.h>
#include <maze/MazeTypeclasses.h>
#include <graphmaze/GraphMaze.h>
#include <graphmaze/GraphMazeTypeclasses.h>
#include <thickmaze/ThickMaze.h>
#include <thickmaze/ThickMazeTypeclasses.h>
using namespace spelunker;
using namespace typeclasses;

// This declares the type to be available everywhere.
template <> class Homomorphism<graphmaze::GraphMaze, thickmaze::ThickMaze> :
        public CompositionMorphism<Homomorphism<graphmaze::GraphMaze, maze::Maze>,
                Homomorphism<maze::Maze, thickmaze::ThickMaze>> {};

int main(int argc, char *argv[]) {
    maze::DFSMazeGenerator gen(50,40);
    maze::Maze m = gen.generate();
    std::cout << Show<spelunker::maze::Maze>::show(m);
    std::cout << std::endl;

    graphmaze::GraphMaze gm = Homomorphism<maze::Maze, graphmaze::GraphMaze>::morph(m);
    std::cout << Show<graphmaze::GraphMaze>::show(gm);
    std::cout << std::endl;

    spelunker::maze::Maze m2 = Homomorphism<graphmaze::GraphMaze, maze::Maze>::morph(gm);
    std::cout << Show<maze::Maze>::show(m2);
    std::cout << std::endl;

    thickmaze::ThickMaze tm2 =
            CompositionMorphism<Homomorphism<graphmaze::GraphMaze, maze::Maze>,
                                Homomorphism<maze::Maze, thickmaze::ThickMaze>>::morph(gm);
    auto tm = Homomorphism<graphmaze::GraphMaze, thickmaze::ThickMaze>::morph(gm);
    std::cout << spelunker::typeclasses::Show<spelunker::thickmaze::ThickMaze>::show(tm);
    return 0;
}