/**
 * app_test_furthest_cells.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <iostream>
#include <types/CommonMazeAttributes.h>
#include <typeclasses/Show.h>
#include <maze/Maze.h>
#include <maze/MazeTypeclasses.h>
#include <maze/DFSMazeGenerator.h>
using namespace spelunker;

int main() {
    const auto m = maze::DFSMazeGenerator{100, 100}.generate();
    const auto distInfo = m.findDiameter();
    std::cout << typeclasses::Show<maze::Maze>::show(m);
    std::cout << std::endl;


    std::cout << "Maximum distance is: " << distInfo.distance << std::endl;
    for (const auto cells: distInfo.cellList) {
        const auto [c1, c2] = cells;
        std::cout << '\t' << typeclasses::Show<types::Cell>::show(c1)
                  << " - " << typeclasses::Show<types::Cell>::show(c2) << std::endl;
    }
}