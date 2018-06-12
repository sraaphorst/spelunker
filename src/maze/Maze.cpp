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
#include <stdexcept>
#include <tuple>
#include <vector>

#include "types/CommonMazeAttributes.h"
#include "types/Exceptions.h"
#include "MazeAttributes.h"
#include "math/MathUtils.h"
#include "math/RNG.h"
#include "Maze.h"
#include "MazeGenerator.h"

namespace spelunker::maze {
    Maze::Maze(const int w,
               const int h,
               const types::PossibleCell &s,
               const types::CellCollection &ends,
               const WallIncidence &walls)
            : width(w), height(h), numWalls(calculateNumWalls(w, h)),
              startCell(s), endingCells(ends), wallIncidence(walls) {
        if (width < 1 || height < 1)
            throw types::IllegalDimensions(width, height);
        checkCells();
    }

    Maze::Maze(const int w,
               const int h,
               const WallIncidence &walls)
            : Maze(w, h, {}, types::CellCollection(), walls) {}

    const Maze Maze::withStartingCell(const types::PossibleCell &s) const {
        return Maze(width, height, s, endingCells, wallIncidence);
    }

    const Maze Maze::withEndingCells(const types::CellCollection &ends) const {
        return Maze(width, height, startCell, ends, wallIncidence);
    }

    bool Maze::wall(const types::Position &p) const noexcept {
        const int rk = rankPosition(p);
        return rk == -1 || wallIncidence[rk];
    }

    bool Maze::wall(int x, int y, types::Direction d) const noexcept {
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

    int Maze::numCellWalls(const spelunker::types::Cell &c) const {
        return numCellWallsInWI(c, wallIncidence);
    }

    const Maze Maze::applySymmetry(const types::Symmetry s) const {
        // Get the symmetry map corresponding to the symmetry.
        std::function<WallID(const types::Position &)> mp;

        switch (s) {
            case types::ROTATION_BY_90:
                mp = [this, s](const types::Position &p) {
                    auto[c, d] = p;
                    auto[x, y] = c;
                    return Maze::rankPositionS(height, width, height - y - 1, x, types::applySymmetryToDirection(s, d));
                };
                break;
            case types::ROTATION_BY_180:
                mp = [this, s](const types::Position &p) {
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
                if (width != height) throw types::IllegalGroupOperation(width, height, s);
                mp = [this, s](const types::Position &p) {
                    auto[c, d] = p;
                    auto[x, y] = c;
                    return Maze::rankPositionS(height, width, y, x, types::applySymmetryToDirection(s, d));
                };
                break;
            case types::REFLECTION_IN_NESW:
                if (width != height) throw types::IllegalGroupOperation(width, height, s);
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
        auto nwi = WallIncidence(numWalls, true);
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

    const Maze Maze::makeUnicursal() const {
        // We need a wall incidence map of size 2w x 2h.
        const int uw = 2 * width;
        const int uh = 2 * height;
        const int uNumWalls = calculateNumWalls(uw, uh);

        // We start off empty and use this maze to place walls in the unicursal new one.
        auto wi = WallIncidence(uNumWalls, false);

        // Boundary walls already exist, and trying to add them would be erroneous.
        const int firstcol = 0;
        const int lastcol = width - 1;
        const int firstrow = 0;
        const int lastrow = height - 1;

        // Shorten ranking for new maze.
        auto ranker = [uw, uh](const int x, const int y, const types::Direction d) {
            return rankPositionS(uw, uh, x, y, d);
        };

        for (auto y = 0; y < height; ++y) {
            auto y2 = 2 * y;
            for (auto x = 0; x < width; ++x) {
                auto x2 = 2 * x;

                // Get the four walls of the original cell.
                const bool n = wall(x, y, types::NORTH);
                const bool e = wall(x, y, types::EAST);
                const bool s = wall(x, y, types::SOUTH);
                const bool w = wall(x, y, types::WEST);

                // Process if the cell has a north wall.
                if (n) {
                    if (y != firstrow) {
                        wi[ranker(x2, y2, types::NORTH)] = true;
                        wi[ranker(x2 + 1, y2, types::NORTH)] = true;
                    }
                    if (!w) wi[ranker(x2, y2 + 1, types::NORTH)] = true;
                    if (!e) wi[ranker(x2 + 1, y2 + 1, types::NORTH)] = true;
                    if (!(e || s || w)) wi[ranker(x2, y2 + 1, types::EAST)] = true;
                }

                // Process if the cell has a south wall.
                if (s) {
                    if (y != lastrow) {
                        wi[ranker(x2, y2 + 1, types::SOUTH)] = true;
                        wi[ranker(x2 + 1, y2 + 1, types::SOUTH)] = true;
                    }
                    if (!w) wi[ranker(x2, y2, types::SOUTH)] = true;
                    if (!e) wi[ranker(x2 + 1, y2, types::SOUTH)] = true;
                    if (!(n || e || w)) wi[ranker(x2, y2, types::EAST)] = true;
                }

                // Process if the cell has a west wall.
                if (w) {
                    if (x != firstcol) {
                        wi[ranker(x2, y2, types::WEST)] = true;
                        wi[ranker(x2, y2 + 1, types::WEST)] = true;
                    }
                    if (!n) wi[ranker(x2 + 1, y2, types::WEST)] = true;
                    if (!s) wi[ranker(x2 + 1, y2 + 1, types::WEST)] = true;
                    if (!(n || e || s)) wi[ranker(x2 + 1, y2, types::SOUTH)] = true;
                }

                // Process if the cell has an east wall.
                // Process if the cell has a west wall.
                if (e) {
                    if (x != lastcol) {
                        wi[ranker(x2 + 1, y2, types::EAST)] = true;
                        wi[ranker(x2 + 1, y2 + 1, types::EAST)] = true;
                    }
                    if (!n) wi[ranker(x2, y2, types::EAST)] = true;
                    if (!s) wi[ranker(x2, y2 + 1, types::EAST)] = true;
                    if (!(n || s || w)) wi[ranker(x2, y2, types::SOUTH)] = true;
                }

                // If no walls, put a cross in the middle.
                if (!(n || e || s || w)) {
                    wi[ranker(x2, y2, types::EAST)] = true;
                    wi[ranker(x2, y2, types::SOUTH)] = true;
                    wi[ranker(x2 + 1, y2 + 1, types::WEST)] = true;
                    wi[ranker(x2 + 1, y2 + 1, types::NORTH)] = true;
                }
            }
        }

        // If the original maze had an entrance, on a border, we make this the entrance + exit.
        types::PossibleCell uStart{};

        if (startCell.has_value()) {
            auto[sx, sy] = startCell.value();
            if (sx == 0 || sx == lastcol) {
                wi[ranker(sx, sy, types::EAST)] = true;
                uStart = {types::cell(sx, sy)};
            } else if (sy == 0 || sy == lastrow) {
                wi[ranker(sx, sy, types::SOUTH)] = true;
                uStart = {types::cell(sx, sy)};
            }
        }

        return Maze(uw, uh, uStart, types::CellCollection(), wi);
    }

    const Maze Maze::braid(const double probability) const {
        math::MathUtils::checkProbability(probability);

        // Create a copy of the wall incidence for this maze for the new maze.
        WallIncidence wi = wallIncidence;

        // Find all the dead ends and store them, shuffle them, and process.
        types::CellCollection deadends = findDeadEnds();
        math::RNG::shuffle(deadends);

        for (auto c: deadends) {
            // Check that the probability succeeds and that this cell is still a dead end.
            if (math::RNG::randomProbability() > probability || numCellWallsInWI(c, wi) < 3)
                continue;

            // Create a list of the wall ranks pointing to the valid cells with the most walls.
            std::vector<WallID> candidates;
            auto maxWalls = 0;
            for (auto d: types::directions()) {
                const auto pos = types::pos(c, d);
                const auto rk = rankPosition(pos);

                // We need to actually have a wall to remove.
                if (rk < 0 || !wi[rk])
                    continue;

                const auto nbrOpt = evaluatePosition(pos);
                if (!nbrOpt.has_value())
                    continue;

                // We have a valid neighbour.
                const auto nbr = nbrOpt.value();
                const auto nbrWalls = numCellWallsInWI(nbr, wi);
                if (nbrWalls < maxWalls)
                    continue;

                if (nbrWalls > maxWalls) {
                    candidates.clear();
                    maxWalls = nbrWalls;
                }
                candidates.emplace_back(rk);
            }

            // Now pick a random neighbour.
            const auto nbr = math::RNG::randomElement(candidates);
            wi[nbr] = false;
        }

        return Maze(width, height, startCell, endingCells, wi);
    }

    const types::CellCollection Maze::findDeadEnds() const noexcept {
        types::CellCollection deadends;
        for (auto y = 0; y < height; ++y) {
            for (auto x = 0; x < width; ++x) {
                const auto c = types::cell(x, y);
                if (numCellWalls(c) == 3)
                    deadends.emplace_back(c);
            }
        }
        return deadends;
    }

    int Maze::numCellWallsInWI(const spelunker::types::Cell &c, const spelunker::maze::WallIncidence &wi) const {
        checkCell(c);

        int num = 0;
        for (auto d: types::directions()) {
            // Note that we can't use any of the wall methods here because those will work with the wrong WallIncidence.
            const auto rk = rankPosition(types::pos(c, d));
            if (rk < 0 || wi[rk])
                ++num;
        }
        return num;
    }

    const types::PossibleCell Maze::evaluatePosition(const types::Position &p) const noexcept {
        const auto[c, d]  = p;
        const auto[x, y]  = c;
        int newX = x;
        int newY = y;

        switch (d) {
            case types::NORTH:
                --newY;
                break;
            case types::EAST:
                ++newX;
                break;
            case types::SOUTH:
                ++newY;
                break;
            case types::WEST:
                --newX;
                break;
        }

        if (newX < 0 || newX >= width || newY < 0 || newY >= height)
            return {};
        else
            return types::cell(newX, newY);
    }

    WallID Maze::rankPosition(const types::Position &p) const {
        const auto &cell = p.first;
        const auto x = cell.first;
        const auto y = cell.second;
        const auto d = p.second;

        return rankPositionS(width, height, x, y, d);
    };

    WallID Maze::rankPosition(int x, int y, types::Direction d) const {
        return rankPositionS(width, height, x, y, d);
    }

    WallID Maze::rankPositionS(const int w,
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

        // Make the compiler happy.
        throw std::invalid_argument("Trying to rank illegal direction.");
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
            throw types::OutOfBoundsCell(types::Cell(x, y));
    }

#ifndef NDEBUG

    void Maze::test_rankPositionS(const int w, const int h) {
        std::set<int> ranks;
        const auto numwalls = calculateNumWalls(w, h);

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