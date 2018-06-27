/**
 * app_test_roomfinder.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <iostream>

#include <thickmaze/ThickMaze.h>
#include <thickmaze/ThickMazeTypeclasses.h>
#include <thickmaze/CellularAutomatonThickMazeGenerator.h>
#include <types/AbstractMaze.h>
#include <typeclasses/Homomorphism.h>
#include <squashedmaze/RoomFinder.h>

using namespace spelunker;

int main() {
    constexpr auto width  = 100;
    constexpr auto height = 100;

    const auto gen = thickmaze::CellularAutomatonThickMazeGenerator{width, height};
    const auto tm = gen.generate();

    std::cout << typeclasses::Show<thickmaze::ThickMaze>::show(tm) << endl;

    const auto &am = dynamic_cast<const types::AbstractMaze<thickmaze::ThickMaze> &>(tm);
    const squashedmaze::RoomFinder f(am);
    const auto &cellToRoom = f.getCellToRoom();
    const auto &roomContents = f.getRoomContents();
}