/**
 * recursive_division.cpp
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Uses the recursive division algorithm, which is highly configurable, to create a maze.
 */

#include <iostream>

#include "Executor.h"
#include "maze/Maze.h"
#include "typeclasses/Show.h"
#include "maze/RecursiveDivisionMazeGenerator.h"
#include "Utils.h"

int main(int argc, char *argv[]) {
    return Executor<spelunker::maze::RecursiveDivisionMazeGenerator>::generateAndDisplayMaze(argc, argv);
}
