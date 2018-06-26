/**
 * app_test_squishedmaze.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <maze/DFSMazeGenerator.h>
#include <maze/Maze.h>
#include <squashedmaze/SquashedMazeAttributes.h>
#include <squashedmaze/SquashedMaze.h>

using namespace spelunker;

int main() {
    constexpr auto width = 20;
    constexpr auto height = 20;
    const maze::DFSMazeGenerator dfs{width, height};
    const auto m = dfs.generate();

    const auto sm = squashedmaze::SquashedMaze(m);
}