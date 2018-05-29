/**
 * Maze.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <cassert>
#include <functional>
#include <map>
#include <optional>
#include <set>
#include <tuple>
#include <vector>

#include "Exceptions.h"
#include "MazeAttributes.h"
#include "Maze.h"

namespace spelunker::maze {
    Maze::Maze(const int w,
               const int h,
               const PossibleStartCell &s,
               const CellCollection &ends,
               const WallIncidence &walls)
            : width(w), height(h), numWalls(calculateNumWalls(w, h)),
              startCell(s), endingCells(ends), wallIncidence(walls) {
        if (width < 1 || height < 1)
            throw IllegalDimensions(width, height);
        checkCells();
    }

    Maze::Maze(const int w,
               const int h,
               const WallIncidence &walls)
            : Maze(w, h, {}, CellCollection(), walls) {}

    const Maze Maze::withStartingCell(const PossibleStartCell &s) const {
        return Maze(width, height, s, endingCells, wallIncidence);
    }

    const Maze Maze::withEndingCells(const CellCollection &ends) const {
        return Maze(width, height, startCell, ends, wallIncidence);
    }

    bool Maze::wall(const Position &p) const noexcept {
        const int rk = rankPosition(p);
        return rk == -1 || wallIncidence[rk];
    }

    bool Maze::wall(int x, int y, Direction d) const noexcept {
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

    const Maze Maze::applySymmetry(Symmetry s) const {
        // Get the symmetry map corresponding to the symmetry.
        std::function<WallID(const Position&)> mp;

        switch (s) {
            case ROTATION_BY_90:
                mp = [this, s](const Position &p) {
                    auto[c, d] = p;
                    auto[x, y] = c;
                    return Maze::rankPositionS(height, width, height - y - 1, x, applySymmetryToDirection(s, d));
                };
                break;
            case ROTATION_BY_180:
                mp =  [this, s](const Position &p) {
                    auto[c, d] = p;
                    auto[x, y] = c;
                    return Maze::rankPositionS(width, height, width - x - 1, height - y - 1,
                                               applySymmetryToDirection(s, d));
                };
                break;
            case ROTATION_BY_270:
                mp = [this, s](const Position &p) {
                    auto[c, d] = p;
                    auto[x, y] = c;
                    return Maze::rankPositionS(height, width, y, width - x - 1, applySymmetryToDirection(s, d));
                };
                break;
            case REFLECTION_IN_X:
                mp = [this, s](const Position &p) {
                    auto[c, d] = p;
                    auto[x, y] = c;
                    return Maze::rankPositionS(width, height, x, height - y - 1, applySymmetryToDirection(s, d));
                };
                break;
            case REFLECTION_IN_Y:
                mp = [this, s](const Position &p) {
                    auto[c, d] = p;
                    auto[x, y] = c;
                    return Maze::rankPositionS(width, height, width - x - 1, y, applySymmetryToDirection(s, d));
                };
                break;
            case REFLECTION_IN_NWSE:
                if (width != height) throw IllegalGroupOperation(width, height, s);
                mp = [this, s](const Position &p) {
                    auto[c, d] = p;
                    auto[x, y] = c;
                    return Maze::rankPositionS(height, width, y, x, applySymmetryToDirection(s, d));
                };
                break;
            case REFLECTION_IN_NESW:
                if (width != height) throw IllegalGroupOperation(width, height, s);
                mp = [this, s](const Position &p) {
                    auto[c, d] = p;
                    auto[x, y] = c;
                    return Maze::rankPositionS(height, width, height - y - 1, width - x - 1,
                                               applySymmetryToDirection(s, d));
                };
                break;
        }

        // Determine the new width / height and create the wall incidence.
        int nWidth, nHeight;
        if (s == ROTATION_BY_180 || s == REFLECTION_IN_X || s == REFLECTION_IN_Y) {
            nWidth = width;
            nHeight = height;
        } else {
            nWidth = height;
            nHeight = width;
        }

        const auto dirs = directions();
        auto nwi = WallIncidence(numWalls, true);
        for (auto x = 0; x < width; ++x)
            for (auto y = 0; y < height; ++y)
                for (auto d : dirs) {
                    auto p = pos(x, y, d);
                    auto rk = rankPosition(p);
                    if (rk == -1) continue;

                    auto nRk = mp(p);
                    nwi[nRk] = wallIncidence[rk];
                }

        return Maze(nWidth, nHeight, nwi);
    }

    const Maze Maze::makeUnicursal() const {
        // We need a wall incidence map of size 2w x 2h.
        const int uw = 2 * width;
        const int uh = 2 * height;
        const int uNumWalls = calculateNumWalls(uw, uh);

        // We start off empty and use this maze to place walls in the unicursal new one.
        auto wi = WallIncidence(uNumWalls, false);

        // Boundary walls already exist, and trying to add them would be erroneous.
        const int firstcol = 0;
        const int lastcol  = width - 1;
        const int firstrow = 0;
        const int lastrow  = height - 1;

        // Shorten ranking for new maze.
        auto ranker = [uw,uh](const int x, const int y, const Direction d) { return rankPositionS(uw, uh, x, y, d); };

        for (auto y = 0; y < height; ++y) {
            auto y2 = 2 * y;
            for (auto x = 0; x < width; ++x) {
                auto x2 = 2 * x;

                // Get the four walls of the original cell.
                const bool n = wall(x, y, NORTH);
                const bool e = wall(x, y, EAST);
                const bool s = wall(x, y, SOUTH);
                const bool w = wall(x, y, WEST);

                // Process if the cell has a north wall.
                if (n) {
                    if (y != firstrow) {
                                        wi[ranker(  x2,   y2, NORTH)] = true;
                                        wi[ranker(x2+1,   y2, NORTH)] = true;
                    }
                    if (!w)             wi[ranker(  x2, y2+1, NORTH)] = true;
                    if (!e)             wi[ranker(x2+1, y2+1, NORTH)] = true;
                    if (!(e || s || w)) wi[ranker(  x2, y2+1, EAST)]  = true;
                }

                // Process if the cell has a south wall.
                if (s) {
                    if (y != lastrow) {
                                        wi[ranker(  x2, y2+1, SOUTH)] = true;
                                        wi[ranker(x2+1, y2+1, SOUTH)] = true;
                    }
                    if (!w)             wi[ranker(  x2,   y2, SOUTH)] = true;
                    if (!e)             wi[ranker(x2+1,   y2, SOUTH)] = true;
                    if (!(n || e || w)) wi[ranker(  x2,   y2, EAST)]  = true;
                }

                // Process if the cell has a west wall.
                if (w) {
                    if (x != firstcol) {
                                        wi[ranker(  x2,   y2, WEST)]  = true;
                                        wi[ranker(  x2, y2+1, WEST)]  = true;
                    }
                    if (!n)             wi[ranker(x2+1,   y2, WEST)]  = true;
                    if (!s)             wi[ranker(x2+1, y2+1, WEST)]  = true;
                    if (!(n || e || s)) wi[ranker(x2+1,   y2, SOUTH)] = true;
                }

                // Process if the cell has an east wall.
                // Process if the cell has a west wall.
                if (e) {
                    if (x != lastcol) {
                                        wi[ranker(x2+1,   y2, EAST)]  = true;
                                        wi[ranker(x2+1, y2+1, EAST)]  = true;
                    }
                    if (!n)             wi[ranker(  x2,   y2, EAST)]  = true;
                    if (!s)             wi[ranker(  x2, y2+1, EAST)]  = true;
                    if (!(n || s || w)) wi[ranker(  x2,   y2, SOUTH)] = true;
                }

                // If no walls, put a cross in the middle.
                if (!(n || e || s || w)) {
                    wi[ranker(  x2,   y2, EAST)]  = true;
                    wi[ranker(  x2,   y2, SOUTH)] = true;
                    wi[ranker(x2+1, y2+1, WEST)]  = true;
                    wi[ranker(x2+1, y2+1, NORTH)] = true;
                }
            }
        }

        // If the original maze had an entrance, on a border, we make this the entrance + exit.
        PossibleStartCell uStart = {};

        if (startCell.has_value()) {
            auto [sx, sy] = startCell.value();
            if (sx == 0 || sx == lastcol) {
                wi[ranker(sx, sy, EAST)]  = true;
                uStart = { cell(sx, sy) };
            }
            else if (sy == 0 || sy == lastrow) {
                wi[ranker(sx, sy, SOUTH)] = true;
                uStart = { cell(sx, sy) };
            }
        }

        return Maze(uw, uh, uStart, CellCollection(), wi);
    }

    WallID Maze::rankPosition(const Position &p) const {
        const auto &cell = p.first;
        const auto x = cell.first;
        const auto y = cell.second;
        const auto d = p.second;

        return rankPositionS(width, height, x, y, d);
    };

    WallID Maze::rankPosition(int x, int y, Direction d) const {
        return rankPositionS(width, height, x, y, d);
    }

    WallID Maze::rankPositionS(const int w,
                                      const int h,
                                      const int x,
                                      const int y,
                                      const Direction d) {
        // Check the position for validity.
        checkCell(w, h, x, y);

        // Get rid of all the boundary cases first to simplify, as these are easy to identify.
        if (x == 0 && d == WEST) return -1;
        if (x == (w - 1) && d == EAST) return -1;
        if (y == 0 && d == NORTH) return -1;
        if (y == (h - 1) && d == SOUTH) return -1;

        // Handle north / south walls first.
        if (d == SOUTH)
            return y * w + x;
        if (d == NORTH)
            return (y - 1) * w + x;

        // Now handle east / west walls.
        const auto offset = w * (h - 1);
        if (d == EAST)
            return x * h + y + offset;
        if (d == WEST)
            return (x - 1) * h + y + offset;

    }

    void Maze::checkCells() const {
        if (startCell)
            checkCell(*startCell);
        for (auto p: endingCells)
            checkCell(p);
    }

    void Maze::checkCell(const Cell &c) const {
        return checkCell(width, height, c.first, c.second);
    }

    void Maze::checkCell(const int w, const int h, const int x, const int y) {
        if (x < 0 || x > w || y < 0 || y >= h)
            throw OutOfBoundsCell(Cell(x, y));
    }

#ifndef NDEBUG

    void Maze::test_rankPositionS(const int w, const int h) {
        std::set<int> ranks;
        const auto numwalls = calculateNumWalls(w, h);

        for (auto x = 0; x < w; ++x)
            for (auto y = 0; y < h; ++y) {
                for (auto d: directions()) {
                    const auto r = rankPositionS(w, h, x, y, d);
                    if (r != -1) ranks.insert(r);
                }
                if (y > 0 && y < h - 1)
                    assert(rankPositionS(w, h, x, y, NORTH) == rankPositionS(w, h, x, y - 1, SOUTH));
                if (x > 0 && x < w - 1)
                    assert(rankPositionS(w, h, x - 1, y, EAST) == rankPositionS(w, h, x, y, WEST));
            }

        for (auto i = 0; i < numwalls; ++i)
            assert(ranks.find(i) != ranks.end());
    }

#endif
}