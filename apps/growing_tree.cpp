/**
 * growing_tree.cpp
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Generate a maze using the growing tree algorithm.
 * Because @see{GrowingTreeMazeGenerator} takes a third parameter indicating the selection
 * method, we must implement without the use of @see{Executor}.
 */

#include <iostream>

#include "Maze.h"
#include "Show.h"
#include "GrowingTreeMazeGenerator.h"
#include "Utils.h"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " width height strategy, where:" << std::endl
                  << "\tstrategy: technique used to choose cells to extend" << std::endl
                  << "\t\t0: oldest cell first" << std::endl
                  << "\t\t1: newest cell first" << std::endl
                  << "\t\t2: middle cell first" << std::endl
                  << "\t\t3: random cell first" << std::endl;
        return 1;
    }

    const int width = Utils::parseLong(argv[1]);
    if (width <= 0) {
        std::cerr << "Invalid width: " << argv[1] << std::endl;
        return 2;
    }

    const int height = Utils::parseLong(argv[2]);
    if (height <= 0) {
        std::cerr << "Invalid height: " << argv[2] << std::endl;
        return 3;
    }

    const int strategy = Utils::parseLong(argv[3]);
    if (strategy < 0 || strategy > 3) {
        std::cerr << "Invalid strategy: " << argv[3] << std::endl;
        return 4;
    }

    // Retrieve the strategy.
    spelunker::maze::GrowingTreeMazeGenerator::CellSelectionStrategy strategyType;
    switch (strategy) {
        case 0:
            strategyType = spelunker::maze::GrowingTreeMazeGenerator::OLDEST;
            break;
        case 1:
            strategyType = spelunker::maze::GrowingTreeMazeGenerator::NEWEST;
            break;
        case 2:
            strategyType = spelunker::maze::GrowingTreeMazeGenerator::MIDDLE;
            break;
        default:
            strategyType = spelunker::maze::GrowingTreeMazeGenerator::RANDOM;
            break;
    };

    spelunker::maze::GrowingTreeMazeGenerator gen(width, height, strategyType);
    const spelunker::maze::Maze m = gen.generate();
    std::cout << spelunker::typeclasses::Show<spelunker::maze::Maze>::show(m);
    return 0;
}