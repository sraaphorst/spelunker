/**
 * binarytree.cpp
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Generate a maze using the binary tree  algorithm.
 */

#include <iostream>

#include "Maze.h"
#include "Show.h"
#include "BinaryTreeMazeGenerator.h"
#include "Utils.h"

int main(int argc, char *argv[]) {
    if (argc != 3 && argc != 4) {
        std::cerr << "Usage: " << argv[0] << " width height [probability], where:" << std::endl
                  << "\tprobability: probability per cell, when possible, of burrowing east instead of south (default 0.5)" << std::endl;
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
    if (argc == 4) {
        probability = Utils::parseDouble(argv[3]);
        if (probability <= 0 || probability >= 1) {
            std::cerr << "Invalid probability: " << argv[3] << std::endl;
            return 4;
        }
    }

    vorpal::maze::BinaryTreeMazeGenerator gen(width, height, probability);
    const vorpal::maze::Maze m = gen.generate();
    std::cout << vorpal::typeclasses::Show<vorpal::maze::Maze>::show(m);
    return 0;
}
