/**
 * ThickMazeGeneratorByHomomorphism.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <types/Dimensions2D.h>
#include <typeclasses/Homomorphism.h>
#include <maze/Maze.h>
#include <maze/MazeGenerator.h>
#include <maze/MazeTypeclasses.h>

#include "ThickMaze.h"
#include "ThickMazeGeneratorByHomomorphism.h"

namespace spelunker::thickmaze {
    ThickMazeGeneratorByHomomorphism::ThickMazeGeneratorByHomomorphism(const maze::MazeGenerator *mg)
        : ThickMazeGenerator{types::Dimensions2D{2 * mg->getWidth() - 1, 2 * mg->getHeight() - 1}}, mazeGenerator{mg} {}

    ThickMazeGeneratorByHomomorphism::~ThickMazeGeneratorByHomomorphism() {
        delete mazeGenerator;
    }

    const ThickMaze ThickMazeGeneratorByHomomorphism::generate() const noexcept {
        const maze::Maze m = mazeGenerator->generate();
        const ThickMaze tm = typeclasses::Homomorphism<maze::Maze, ThickMaze>::morph(m);
        return tm;
    }
}