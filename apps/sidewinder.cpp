/**
 * sidewinder.cpp
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Generate a maze using the sidewinder algorithm.
 */

#include <maze/SidewinderMazeGenerator.h>

#include "ProbabilisticExecutor.h"

int main(int argc, char *argv[]) {
    return ProbabilisticExecutor<spelunker::maze::SidewinderMazeGenerator>::generateAndDisplayMaze(argc, argv);
}