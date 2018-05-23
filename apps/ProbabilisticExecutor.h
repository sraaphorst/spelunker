/**
 * ProbabilisticExecutor.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Generalized method of reading parameters, creating a random maze, and outputting the result.
 * Accept one probability factor.
 */

#ifndef SPELUNKER_PROBABILISTICEXECUTOR_H
#define SPELUNKER_PROBABILISTICEXECUTOR_H

#include <iostream>

#include "Maze.h"
#include "Show.h"
#include "Utils.h"

template<typename T>
class ProbabilisticExecutor {
private:
    ProbabilisticExecutor() = default;

public:
    /// Generate a Maze using a MazeGenerator (type T), and output it. argv[1] is width, argv[2] is height.
    /**
     * Generate a Maze using a MazeGenerator and display it to the terminal.
     * The size of the maze should be given by the user in argv[1] (width) and argv[2] (height).
     * @tparam T the MazeGenerator to use
     * @param argc command line argument size
     * @param argv command line arguments
     * @return return code for main
     */
    static int generateAndDisplayMaze(int argc, char *argv[]) {
        if (argc != 3 && argc != 4) {
            std::cerr << "Usage: " << argv[0] << " width height [probability]" << std::endl
                      << "\tprobability: allows a probability factor to affect output, default 0.5" << std::endl;
            return 1;
        }

        // Parse the width and height.
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

        T gen(width, height, probability);
        const spelunker::maze::Maze m = gen.generate();
        std::cout << spelunker::typeclasses::Show<spelunker::maze::Maze>::show(m);
        return 0;
    }
};

#endif //SPELUNKER_PROBABILISTICEXECUTOR_H
