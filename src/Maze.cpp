/**
 * Maze.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <cassert>
#include <functional>
#include <map>
#include <optional>
#include <tuple>
#include <vector>

#include "Exceptions.h"
#include "MazeAttributes.h"
#include "Maze.h"

namespace spelunker::maze {
    Maze::Maze(const int w,
               const int h,
               const types::PossibleStartCell &s,
               const types::CellCollection &ends,
               const types::WallIncidence &walls)
            : width(w), height(h), numWalls(types::calculateNumWalls(w, h)),
              startCell(s), endingCells(ends), wallIncidence(walls) {
        if (width < 1 || height < 1)
            throw shared::IllegalDimensions(width, height);
        checkCells();
    }

    Maze::Maze(const int w,
               const int h,
               const spelunker::maze::types::WallIncidence &walls)
            : Maze(w, h, {}, types::CellCollection(), walls) {}

    const Maze Maze::withStartingCell(const types::PossibleStartCell &s) const {
        return Maze(width, height, s, endingCells, wallIncidence);
    }

    const Maze Maze::withEndingCells(const spelunker::maze::types::CellCollection &ends) const {
        return Maze(width, height, startCell, ends, wallIncidence);
    }

    bool Maze::wall(const spelunker::maze::types::Position &p) const noexcept {
        const int rk = rankPosition(p);
        return rk == -1 || wallIncidence[rk];
    }

    bool Maze::wall(int x, int y, spelunker::maze::types::Direction d) const noexcept {
        const int rk = rankPosition(x, y, d);
        return rk == -1 || wallIncidence[rk];
    }

    bool Maze::operator==(const Maze &other) const {
        if (width != other.width || height != other.height)
            return false;

        // We can just compare the wall incidence vectors, since == on vectors of the
        // same size compares the contents.
        return wallIncidence == other.wallIncidence;
    }

    const Maze Maze::applySymmetry(types::Symmetry s) const {
        // Get the symmetry map corresponding to the symmetry.
        std::function<types::WallID(const types::Position&)> mp;

        switch (s) {
            case types::ROTATION_BY_90:
                mp = [this, s](const types::Position &p) {
                    auto[c, d] = p;
                    auto[x, y] = c;
                    return Maze::rankPositionS(height, width, height - y - 1, x, types::applySymmetryToDirection(s, d));
                };
                break;
            case types::ROTATION_BY_180:
                mp =  [this, s](const types::Position &p) {
                    auto[c, d] = p;
                    auto[x, y] = c;
                    return Maze::rankPositionS(width, height, width - x - 1, height - y - 1,
                                               types::applySymmetryToDirection(s, d));
                };
                break;
            case types::ROTATION_BY_270:
                mp = [this, s](const types::Position &p) {
                    auto[c, d] = p;
                    auto[x, y] = c;
                    return Maze::rankPositionS(height, width, y, width - x - 1, types::applySymmetryToDirection(s, d));
                };
                break;
            case types::REFLECTION_IN_X:
                mp = [this, s](const types::Position &p) {
                    auto[c, d] = p;
                    auto[x, y] = c;
                    return Maze::rankPositionS(width, height, x, height - y - 1, types::applySymmetryToDirection(s, d));
                };
                break;
            case types::REFLECTION_IN_Y:
                mp = [this, s](const types::Position &p) {
                    auto[c, d] = p;
                    auto[x, y] = c;
                    return Maze::rankPositionS(width, height, width - x - 1, y, types::applySymmetryToDirection(s, d));
                };
                break;
            case types::REFLECTION_IN_NWSE:
                if (width != height) throw shared::IllegalGroupOperation(width, height, s);
                mp = [this, s](const types::Position &p) {
                    auto[c, d] = p;
                    auto[x, y] = c;
                    return Maze::rankPositionS(height, width, y, x, types::applySymmetryToDirection(s, d));
                };
                break;
            case types::REFLECTION_IN_NESW:
                if (width != height) throw shared::IllegalGroupOperation(width, height, s);
                mp = [this, s](const types::Position &p) {
                    auto[c, d] = p;
                    auto[x, y] = c;
                    return Maze::rankPositionS(height, width, height - y - 1, width - x - 1,
                                               types::applySymmetryToDirection(s, d));
                };
                break;
        }

        // Determine the new width / height and create the wall incidence.
        int nWidth, nHeight;
        if (s == types::ROTATION_BY_180 || s == types::REFLECTION_IN_X || s == types::REFLECTION_IN_Y) {
            nWidth = width;
            nHeight = height;
        } else {
            nWidth = height;
            nHeight = width;
        }

        const auto dirs = types::directions();
        auto nwi = types::WallIncidence(numWalls, true);
        for (auto x = 0; x < width; ++x)
            for (auto y = 0; y < height; ++y)
                for (auto d : dirs) {
                    auto p = types::pos(x, y, d);
                    auto rk = rankPosition(p);
                    if (rk == -1) continue;

                    auto nRk = mp(p);
                    nwi[nRk] = wallIncidence[rk];
                }

        return Maze(nWidth, nHeight, nwi);
    }

    types::WallID Maze::rankPosition(const types::Position &p) const {
        const auto &cell = p.first;
        const auto x = cell.first;
        const auto y = cell.second;
        const auto d = p.second;

        return rankPositionS(width, height, x, y, d);
    };

    types::WallID Maze::rankPosition(int x, int y, spelunker::maze::types::Direction d) const {
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
            throw shared::OutOfBoundsCell(types::Cell(x, y));
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