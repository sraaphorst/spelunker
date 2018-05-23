/**
 * binarytree.cpp
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Generate a maze using the binary tree algorithm.
 */

#include "ProbabilisticExecutor.h"
#include "BinaryTreeMazeGenerator.h"

int main(int argc, char *argv[]) {
    return ProbabilisticExecutor<vorpal::maze::BinaryTreeMazeGenerator>::generateAndDisplayMaze(argc, argv);
}
