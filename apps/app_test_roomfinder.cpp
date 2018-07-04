/**
 * app_test_roomfinder.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <iostream>

#include <maze/Maze.h>
#include <maze/SidewinderMazeGenerator.h>
#include <maze/MazeTypeclasses.h>
#include <thickmaze/ThickMaze.h>
#include <thickmaze/ThickMazeTypeclasses.h>
#include <thickmaze/CellularAutomatonThickMazeGenerator.h>
#include <types/AbstractMaze.h>
#include <typeclasses/Homomorphism.h>
#include <squashedmaze/RoomFinder.h>

using namespace spelunker;

int main() {
    constexpr auto width  = 50;
    constexpr auto height = 50;

    const auto gen = thickmaze::CellularAutomatonThickMazeGenerator{width, height};
    const auto tm = gen.generate();

    std::cout << typeclasses::Show<thickmaze::ThickMaze>::show(tm) << std::endl;

    const auto &atm = dynamic_cast<const types::AbstractMaze<thickmaze::ThickMaze>&>(tm);
    const squashedmaze::RoomFinder ft(atm);
    const auto &tmRoomContents = ft.getRoomContents();
    std::cout << "Rooms:" << std::endl;
    for (const auto &room: tmRoomContents) {
        std::cout << "\tRoom " << room.first << ':';
        for (const auto &c: room.second)
            std::cout << " (" << c.first << ',' << c.second << ')';
        std::cout << std::endl;
    }
    std::cout << std::endl << std::endl << std::endl;

    const auto gen2 = maze::SidewinderMazeGenerator(width, height);
    const auto m = gen2.generate().braidAll();

    std::cout << typeclasses::Show<maze::Maze>::show(m) << std::endl;

    const auto &am = dynamic_cast<const types::AbstractMaze<maze::Maze>&>(m);
    const squashedmaze::RoomFinder f(am);
    const auto &mRoomContents = f.getRoomContents();
    std::cout << "Rooms:" << std::endl;
    for (const auto &room: mRoomContents) {
        std::cout << "\tRoom " << room.first << ':';
        for (const auto &c: room.second)
            std::cout << " (" << c.first << ',' << c.second << ')';
        std::cout << std::endl;
    }
    const auto invalid = m.findInvalidCells();
    for (const auto c: invalid) {
        std::cout << c.first << "," << c.second << std::endl;
    }
    std::cout << std::endl;
}