/**
 * Executor.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Generalized method of reading parameters, creating a random maze, and outputting the result.
 */

#ifndef SPELUNKER_GENERATETEMPLATE_H
#define SPELUNKER_GENERATETEMPLATE_H

//template<typename T>
//int generateAndDisplayMaze(int argc, char *argv[]);


#include <cstdlib>
#include <iostream>

#include "Maze.h"
#include "Show.h"
#include "Executor.h"

template<typename T>
class Executor {
private:
    Executor() {}

    static long parseLong(const char *str) {
        long val = 0;
        char *temp;

        val = strtol(str, &temp, 0);

        if (*temp != '\0')
            return -1;

        return val;
    }

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
        if (argc != 3) {
            std::cerr << "Usage: " << argv[0] << " width height" << std::endl;
            return 1;
        }

        // Parse the width and height.
        const int width = parseLong(argv[1]);
        if (width <= 0) {
            std::cerr << "Invalid width: " << argv[1] << std::endl;
            return 2;
        }

        const int height = parseLong(argv[2]);
        if (height <= 0) {
            std::cerr << "Invalid height: " << argv[2] << std::endl;
            return 3;
        }

        T gen(width, height);
        const vorpal::maze::Maze m = gen.generate();
        std::cout << vorpal::typeclasses::Show<vorpal::maze::Maze>::show(m) << std::endl;
        return 0;
    }
};

#endif //SPELUNKER_GENERATETEMPLATE_H