/**
 * prim.cpp
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Generate a maze using the hunt-and-kill algorithm.
 */

#include "Executor.h"
#include "HuntAndKillMazeGenerator.h"

int main(int argc, char *argv[]) {
    return Executor<spelunker::maze::HuntAndKillMazeGenerator>::generateAndDisplayMaze(argc, argv);
}
