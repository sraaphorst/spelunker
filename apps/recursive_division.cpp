/**
 * recursive_division.cpp
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Uses the recursive division algorithm, which is highly configurable, to create a maze.
 */

#include <iostream>

#include "Maze.h"
#include "Show.h"
#include "RecursiveDivisionMazeGenerator.h"
#include "Utils.h"

int main(int argc, char *argv[]) {
    if (argc != 3 && argc != 7) {
        std::cerr << "Usage: " << argv[0] << " width height [lowerBound upperBound p alt], where:"     << std::endl
                  << "\tlowerBound: the lower bound (density) to consider in splitting, default = 0.0" << std::endl
                  << "\tupperBound: the upper bound (density) to consider in splitting, default = 1.0" << std::endl
                  << "\tp:          the probability of choosing to split vertically,    default = 0.5" << std::endl
                  << "\talt:        0 to pick split directon randomly, 1 to alternate,  default = 0"   << std::endl;
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

    if (argc == 3) {
        spelunker::maze::RecursiveDivisionMazeGenerator gen(width, height);
        const spelunker::maze::Maze m = gen.generate();
        std::cout << spelunker::typeclasses::Show<spelunker::maze::Maze>::show(m);
    } else {
        const double lowerBound = Utils::parseDouble(argv[3]);
        if (lowerBound < 0 || lowerBound > 1) {
            std::cerr << "Invalid lowerBound: " << argv[3] << std::endl;
            return 4;
        }

        const double upperBound = Utils::parseDouble(argv[4]);
        if (upperBound < 0 || upperBound > 1) {
            std::cerr << "Invalid upperBound: " << argv[4] << std::endl;
            return 5;
        }

        if (lowerBound > upperBound) {
            std::cerr << "Invalid range: " << argv[3] << ">" << argv[4] << std::endl;
            return 6;
        }

        const double probability = Utils::parseDouble(argv[5]);
        if (probability < 0 || probability > 1) {
            std::cerr << "Invalid probability: " << argv[5] << std::endl;
            return 7;
        }

        const int alternate = Utils::parseLong(argv[6]);
        if (alternate != 0 && alternate != 1) {
            std::cerr << "Invalid alternate flag: " << argv[6] << std::endl;
            return 8;
        }

        spelunker::maze::RecursiveDivisionMazeGenerator gen(width, height, lowerBound, upperBound, probability, alternate == 1);
        const spelunker::maze::Maze m = gen.generate();
        std::cout << spelunker::typeclasses::Show<spelunker::maze::Maze>::show(m);
    }

    return 0;
}
