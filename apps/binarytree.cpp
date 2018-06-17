/**
 * binarytree.cpp
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Generate a maze using the binary tree algorithm.
 */

#include <maze/BinaryTreeMazeGenerator.h>

#include "ProbabilisticExecutor.h"

int main(int argc, char *argv[]) {
    return ProbabilisticExecutor<spelunker::maze::BinaryTreeMazeGenerator>::generateAndDisplayMaze(argc, argv);
}
