/**
 * test_symmetry.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <types/Symmetry.h>
#include <typeclasses/Show.h>
#include <maze/DFSMazeGenerator.h>
#include <maze/Maze.h>
#include <maze/MazeTypeclasses.h>
using namespace spelunker;

int main(int argc, char *argv[]) {
    maze::DFSMazeGenerator dfs(50, 40);
    maze::Maze m = dfs.generate();
    std::cout << typeclasses::Show<maze::Maze>::show(m);
    maze::Maze m90 = m.applySymmetry(types::Symmetry::IDENTITY);
    std::cout << typeclasses::Show<maze::Maze>::show(m90);
    return 0;
}