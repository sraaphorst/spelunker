/**
 * Maze.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <cassert>
#include <map>
#include <optional>
#include <tuple>
#include <vector>

#include "Exceptions.h"
#include "MazeAttributes.h"
#include "Maze.h"

namespace vorpal::maze {
    Maze::Maze(const int w,
               const int h,
               const types::PossibleStartCell &s,
               const types::CellCollection &ends,
               const types::WallIncidence &walls)
            : width(w), height(h), numWalls(types::calculateNumWalls(w, h)),
              startCell(s), endingCells(ends), wallIncidence(walls) {
        checkCells();
    }

    Maze::Maze(const int w,
               const int h,
               const vorpal::maze::types::WallIncidence &walls)
            : Maze(w, h, {}, types::CellCollection(), walls) {}

    const Maze Maze::withStartingCell(const types::PossibleStartCell &s) const {
        return Maze(width, height, s, endingCells, wallIncidence);
    }

    const Maze Maze::withEndingCells(const vorpal::maze::types::CellCollection &ends) const {
        return Maze(width, height, startCell, ends, wallIncidence);
    }

    bool Maze::wall(const vorpal::maze::types::Position &p) const noexcept {
        return wallIncidence[rankPosition(p)];
    }

    bool Maze::wall(int x, int y, vorpal::maze::types::Direction d) const noexcept {
        return wallIncidence[rankPosition(x, y, d)];
    }

    types::WallID Maze::rankPosition(const types::Position &p) const {
        const auto &cell = p.first;
        const auto x = cell.first;
        const auto y = cell.second;
        const auto d = p.second;

        return rankPositionS(width, height, x, y, d);
    };

    types::WallID Maze::rankPosition(int x, int y, vorpal::maze::types::Direction d) const {
        return rankPositionS(width, height, x, y, d);
    }

    types::WallID Maze::rankPositionS(const int w,
                                      const int h,
                                      const int x,
                                      const int y,
                                      const types::Direction d) {
        // Check the position for validity.
        checkCell(w, h, x, y);

        // Get rid of all the boundary cases first to simplify, as these are easy to identify.
        if (x == 0 && d == types::WEST) return -1;
        if (x == (w - 1) && d == types::EAST) return -1;
        if (y == 0 && d == types::NORTH) return -1;
        if (y == (h - 1) && d == types::SOUTH) return -1;

        // Handle north / south walls first.
        if (d == types::SOUTH)
            return y * w + x;
        if (d == types::NORTH)
            return (y - 1) * w + x;

        // Now handle east / west walls.
        const auto offset = w * (h - 1);
        if (d == types::EAST)
            return x * h + y + offset;
        if (d == types::WEST)
            return (x - 1) * h + y + offset;

    }

    void Maze::checkCells() const {
        if (startCell)
            checkCell(*startCell);
        for (auto p: endingCells)
            checkCell(p);
    }

    void Maze::checkCell(const types::Cell &c) const {
        return checkCell(width, height, c.first, c.second);
    }

    void Maze::checkCell(const int w, const int h, const int x, const int y) {
        if (x < 0 || x > w || y < 0 || y >= h)
            throw OutOfBoundsCell(types::Cell(x,y));
    }

#ifndef NDEBUG
    void Maze::test_rankPositionS(const int w, const int h) {
        std::set<int> ranks;
        const auto numwalls = types::calculateNumWalls(w, h);

        for (auto x = 0; x < w; ++x)
            for (auto y = 0; y < h; ++y) {
                for (auto d: types::directions()) {
                    const auto r = rankPositionS(w, h, x, y, d);
                    if (r != -1) ranks.insert(r);
                }
                if (y > 0 && y < h - 1)
                    assert(rankPositionS(w, h, x, y, types::NORTH) == rankPositionS(w, h, x, y - 1, types::SOUTH));
                if (x > 0 && x < w - 1)
                    assert(rankPositionS(w, h, x - 1, y, types::EAST) == rankPositionS(w, h, x, y, types::WEST));
            }

        for (auto i = 0; i < numwalls; ++i)
            assert(ranks.find(i) != ranks.end());
    }
#endif
}