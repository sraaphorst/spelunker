/**
 * binarytree.cpp
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Generate a maze using the binary tree algorithm.
 */

#include "ProbabilisticExecutor.h"
#include "maze/BinaryTreeMazeGenerator.h"

int main(int argc, char *argv[]) {
    return ProbabilisticExecutor<spelunker::maze::BinaryTreeMazeGenerator>::generateAndDisplayMaze(argc, argv);
}
