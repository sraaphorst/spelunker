/**
 * TestRoomFinder.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <algorithm>

#include <catch.hpp>

#include <thickmaze/ThickMaze.h>
#include <thickmaze/CellularAutomatonThickMazeGenerator.h>
#include <types/AbstractMaze.h>
#include <squashedmaze/RoomFinder.h>

using namespace spelunker;

TEST_CASE("Room finder properly finds all rooms", "[roomfinder][thickmaze]") {
    constexpr auto width = 100;
    constexpr auto height = 100;

    const auto gen = thickmaze::CellularAutomatonThickMazeGenerator{width, height};
    const auto tm = gen.generate();
    const auto invalidCells = tm.findInvalidCells();

    const squashedmaze::RoomFinder f(tm);
    const auto &cellToRoom = f.getCellToRoom();
    const auto &roomContents = f.getRoomContents();

    SECTION("Inaccessible cells do not belong to any room") {
        for (const auto c: invalidCells) {
            const auto[cx, cy] = c;
            REQUIRE(cellToRoom[cx][cy] == -1);
        }
    }

    SECTION("Rooms are linearly ordered with no gaps") {
        std::vector<int> keys;
        std::transform(roomContents.begin(), roomContents.end(), back_inserter(keys),
                       [](const auto &r) { return r.first; });
        const auto iter = std::max_element(keys.begin(), keys.end());
        if (iter != keys.end())
            REQUIRE(*iter == (roomContents.size() - 1));
        else
            REQUIRE(0 == roomContents.size());
    }

    SECTION("Rooms have size >= 4") {
        std::for_each(roomContents.begin(), roomContents.end(), [](const auto &r) {
            const auto sz = r.second.size();
            REQUIRE(sz >= 4);
        });
    }

    SECTION("Cell room mapping and room contents must be consistent") {
        for (auto y = 0; y < height; ++y)
            for (auto x = 0; x < width; ++x) {
                const int roomId = cellToRoom[x][y];
                if (roomId == -1) continue;
                const auto &contents = roomContents.at(roomId);
                REQUIRE_FALSE(std::find(contents.begin(), contents.end(), types::cell(x, y)) == contents.end());
            }
    }
}