/**
 * eller.cpp
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Generate a maze using the Eller algorithm.
 * Because @see{EllerMazeGenerator} takes two extra parameters, we must implement this
 * without the use of @see{Executor}.
 */

#include <iostream>

#include "Maze.h"
#include "Show.h"
#include "EllerMazeGenerator.h"
#include "Utils.h"

int main(int argc, char *argv[]) {
    if (argc != 3 && argc != 5) {
        std::cerr << "Usage: " << argv[0] << " width height [probability density], where:" << std::endl
                  << "\tprobability: probability per row that two horizontally adjacent sets are joined (default 0.5)" << std::endl
                  << "\tdensity:     [1,max(1, density * (set size))] vertical gaps are formed per set in a row" << std::endl;
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

    double probability = 0.5;
    double density = 0.5;
    if (argc == 5) {
        probability = Utils::parseDouble(argv[3]);
        if (probability <= 0 || probability >= 1) {
            std::cerr << "Invalid probability: " << argv[3] << std::endl;
            return 4;
        }

        density = Utils::parseDouble(argv[4]);
        if (density <= 0 || density > 1) {
            std::cerr << "Invalid density: " << argv[4] << std::endl;
            return 5;
        }
    }

    spelunker::maze::EllerMazeGenerator gen(width, height, probability, density);
    const spelunker::maze::Maze m = gen.generate();
    std::cout << spelunker::typeclasses::Show<spelunker::maze::Maze>::show(m);
    return 0;
}
