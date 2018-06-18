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

#include <math/MathUtils.h>
#include <math/RNG.h>
#include <types/AbstractMaze.h>
#include <types/CommonMazeAttributes.h>
#include <types/Direction.h>
#include <types/Exceptions.h>
#include <types/Symmetry.h>
#include "MazeAttributes.h"
#include "Maze.h"
#include "MazeGenerator.h"

#include <iostream>
using namespace std;

namespace spelunker::maze {
    Maze::Maze(const types::Dimensions2D &d,
               const types::PossibleCell &start,
               const types::CellCollection &goals,
               const WallIncidence &walls)
        : AbstractMaze{d, start, goals},
          numWalls{calculateNumWalls(d)},
          wallIncidence{walls} {}

    Maze::Maze(const types::Dimensions2D &d,
               const WallIncidence &walls)
        : Maze{d, {}, types::CellCollection(), walls} {}

    Maze::Maze(const int w,
               const int h,
               const types::PossibleCell &start,
               const types::CellCollection &goals,
               const WallIncidence &walls)
            : Maze{types::Dimensions2D{w, h}, start, goals, walls} {}

    Maze::Maze(const int w,
               const int h,
               const WallIncidence &walls)
            : Maze(w, h, {}, types::CellCollection(), walls) {}

    bool Maze::wall(const types::Position &p) const {
        const int rk = rankPosition(p);
        return rk == -1 || wallIncidence[rk];
    }

    bool Maze::wall(int x, int y, types::Direction d) const {
        const int rk = rankPosition(x, y, d);
        return rk == -1 || wallIncidence[rk];
    }

    bool Maze::operator==(const Maze &other) const noexcept {
        // We can just compare the wall incidence vectors, since == on vectors of the
        // same size compares the contents.
        return getDimensions() == other.getDimensions()
            && wallIncidence == other.wallIncidence;
    }

    int Maze::numCellWalls(const spelunker::types::Cell &c) const {
        return numCellWallsInWI(c, wallIncidence);
    }

    const Maze Maze::applySymmetry(const types::Symmetry s) const {
        // Get the new dimensions for the symmetry.
        const auto ndim = types::applySymmetryToDimensions(s, getDimensions());

        // Get the symmetry map corresponding to the symmetry.
        std::function<WallID(const types::Position &)> mp;

        switch (s) {
            case types::Symmetry::ROTATION_BY_90:
                mp = [this, s, &ndim](const types::Position &p) {
                    const auto[c, d] = p;
                    const auto[x, y] = c;
                    return Maze::rankPositionS(ndim, getHeight() - y - 1, x,
                                               types::applySymmetryToDirection(s, d));
                };
                break;
            case types::Symmetry::ROTATION_BY_180:
                mp = [this, s, &ndim](const types::Position &p) {
                    const auto[c, d] = p;
                    const auto[x, y] = c;
                    return Maze::rankPositionS(ndim, getWidth() - x - 1, getHeight() - y - 1,
                                               types::applySymmetryToDirection(s, d));
                };
                break;
            case types::Symmetry::ROTATION_BY_270:
                mp = [this, s, &ndim](const types::Position &p) {
                    const auto[c, d] = p;
                    const auto[x, y] = c;
                    return Maze::rankPositionS(ndim, y, getWidth() - x - 1,
                                               types::applySymmetryToDirection(s, d));
                };
                break;
            case types::Symmetry::REFLECTION_IN_X:
                mp = [this, s, &ndim](const types::Position &p) {
                    const auto[c, d] = p;
                    const auto[x, y] = c;
                    return Maze::rankPositionS(ndim, x, getHeight() - y - 1,
                                               types::applySymmetryToDirection(s, d));
                };
                break;
            case types::Symmetry::REFLECTION_IN_Y:
                mp = [this, s, &ndim](const types::Position &p) {
                    const auto[c, d] = p;
                    const auto[x, y] = c;
                    return Maze::rankPositionS(ndim, getWidth() - x - 1, y,
                                               types::applySymmetryToDirection(s, d));
                };
                break;
            case types::Symmetry::REFLECTION_IN_NWSE:
                if (!getDimensions().isSquare()) throw types::IllegalGroupOperation(getDimensions(), s);
                mp = [this, s, &ndim](const types::Position &p) {
                    const auto[c, d] = p;
                    const auto[x, y] = c;
                    return Maze::rankPositionS(ndim, y, x, types::applySymmetryToDirection(s, d));
                };
                break;
            case types::Symmetry::REFLECTION_IN_NESW:
                if (!getDimensions().isSquare()) throw types::IllegalGroupOperation(getDimensions(), s);
                mp = [this, s, &ndim](const types::Position &p) {
                    const auto[c, d] = p;
                    const auto[x, y] = c;
                    return Maze::rankPositionS(ndim, getHeight() - y - 1, getWidth() - x - 1,
                                               types::applySymmetryToDirection(s, d));
                };
                break;
        }

        // Determine the new width / height and create the wall incidence.
        const auto nDim = types::applySymmetryToDimensions(s, getDimensions());

        const auto dirs = types::directions();
        auto nwi = WallIncidence(numWalls, true);

        const auto [width, height] = getDimensions().values();
        for (auto x = 0; x < width; ++x)
            for (auto y = 0; y < height; ++y)
                for (auto d : dirs) {
                    auto p = types::pos(x, y, d);
                    auto rk = rankPosition(p);
                    if (rk == -1) continue;

                    auto nRk = mp(p);
                    nwi[nRk] = wallIncidence[rk];
                }

        return Maze(nDim, nwi);
    }

    const Maze Maze::makeUnicursal() const {
        // We need a wall incidence map of size 2w x 2h.
        const types::Dimensions2D ud = getDimensions().scale(2);
        const int uNumWalls = calculateNumWalls(ud);

        // We start off empty and use this maze to place walls in the unicursal new one.
        auto wi = WallIncidence(uNumWalls, false);

        // Boundary walls already exist, and trying to add them would be erroneous.
        const int firstcol = 0;
        const int lastcol = getWidth() - 1;
        const int firstrow = 0;
        const int lastrow = getHeight() - 1;

        // Shorten ranking for new maze.
        auto ranker = [&ud](const int x, const int y, const types::Direction d) {
            return rankPositionS(ud, x, y, d);
        };

        const auto [width, height] = getDimensions().values();
        for (auto y = 0; y < height; ++y) {
            auto y2 = 2 * y;
            for (auto x = 0; x < width; ++x) {
                auto x2 = 2 * x;

                // Get the four walls of the original cell.
                const bool n = wall(x, y, types::Direction::NORTH);
                const bool e = wall(x, y, types::Direction::EAST);
                const bool s = wall(x, y, types::Direction::SOUTH);
                const bool w = wall(x, y, types::Direction::WEST);

                // Process if the cell has a north wall.
                if (n) {
                    if (y != firstrow) {
                        wi[ranker(x2, y2, types::Direction::NORTH)] = true;
                        wi[ranker(x2 + 1, y2, types::Direction::NORTH)] = true;
                    }
                    if (!w) wi[ranker(x2, y2 + 1, types::Direction::NORTH)] = true;
                    if (!e) wi[ranker(x2 + 1, y2 + 1, types::Direction::NORTH)] = true;
                    if (!(e || s || w)) wi[ranker(x2, y2 + 1, types::Direction::EAST)] = true;
                }

                // Process if the cell has a south wall.
                if (s) {
                    if (y != lastrow) {
                        wi[ranker(x2, y2 + 1, types::Direction::SOUTH)] = true;
                        wi[ranker(x2 + 1, y2 + 1, types::Direction::SOUTH)] = true;
                    }
                    if (!w) wi[ranker(x2, y2, types::Direction::SOUTH)] = true;
                    if (!e) wi[ranker(x2 + 1, y2, types::Direction::SOUTH)] = true;
                    if (!(n || e || w)) wi[ranker(x2, y2, types::Direction::EAST)] = true;
                }

                // Process if the cell has a west wall.
                if (w) {
                    if (x != firstcol) {
                        wi[ranker(x2, y2, types::Direction::WEST)] = true;
                        wi[ranker(x2, y2 + 1, types::Direction::WEST)] = true;
                    }
                    if (!n) wi[ranker(x2 + 1, y2, types::Direction::WEST)] = true;
                    if (!s) wi[ranker(x2 + 1, y2 + 1, types::Direction::WEST)] = true;
                    if (!(n || e || s)) wi[ranker(x2 + 1, y2, types::Direction::SOUTH)] = true;
                }

                // Process if the cell has an east wall.
                // Process if the cell has a west wall.
                if (e) {
                    if (x != lastcol) {
                        wi[ranker(x2 + 1, y2, types::Direction::EAST)] = true;
                        wi[ranker(x2 + 1, y2 + 1, types::Direction::EAST)] = true;
                    }
                    if (!n) wi[ranker(x2, y2, types::Direction::EAST)] = true;
                    if (!s) wi[ranker(x2, y2 + 1, types::Direction::EAST)] = true;
                    if (!(n || s || w)) wi[ranker(x2, y2, types::Direction::SOUTH)] = true;
                }

                // If no walls, put a cross in the middle.
                if (!(n || e || s || w)) {
                    wi[ranker(x2, y2, types::Direction::EAST)] = true;
                    wi[ranker(x2, y2, types::Direction::SOUTH)] = true;
                    wi[ranker(x2 + 1, y2 + 1, types::Direction::WEST)] = true;
                    wi[ranker(x2 + 1, y2 + 1, types::Direction::NORTH)] = true;
                }
            }
        }

        // If the original maze had an entrance, on a border, we make this the entrance + exit.
        types::PossibleCell uStart{};

        auto start = getStartingCell();
        if (start) {
            auto[sx, sy] = *start;
            if (sx == 0 || sx == lastcol) {
                wi[ranker(sx, sy, types::Direction::EAST)] = true;
                uStart = {types::cell(sx, sy)};
            } else if (sy == 0 || sy == lastrow) {
                wi[ranker(sx, sy, types::Direction::SOUTH)] = true;
                uStart = {types::cell(sx, sy)};
            }
        }

        return Maze(ud, uStart, types::CellCollection(), wi);
    }

    const Maze Maze::braid(const double probability) const noexcept {
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

        return Maze(getDimensions(), getStartingCell(), getGoalCells(), wi);
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
            case types::Direction::NORTH:
                --newY;
                break;
            case types::Direction::EAST:
                ++newX;
                break;
            case types::Direction::SOUTH:
                ++newY;
                break;
            case types::Direction::WEST:
                --newX;
                break;
        }

        if (newX < 0 || newX >= getWidth() || newY < 0 || newY >= getHeight())
            return {};
        else
            return types::cell(newX, newY);
    }

    WallID Maze::rankPosition(const types::Position &p) const {
        const auto &cell = p.first;
        const auto x = cell.first;
        const auto y = cell.second;
        const auto d = p.second;

        return rankPositionS(getDimensions(), x, y, d);
    };

    WallID Maze::rankPosition(int x, int y, types::Direction d) const {
        return rankPositionS(getDimensions(), x, y, d);
    }

    WallID Maze::rankPositionS(const types::Dimensions2D &dim,
                               const int x,
                               const int y,
                               const types::Direction dir) {
        // Check the position for validity.
        dim.checkCell(x, y);

        // Get rid of all the boundary cases first to simplify, as these are easy to identify.
        const auto [w, h] = dim.values();

        if (x == 0 && dir == types::Direction::WEST) return -1;
        if (x == (w - 1) && dir == types::Direction::EAST) return -1;
        if (y == 0 && dir == types::Direction::NORTH) return -1;
        if (y == (h - 1) && dir == types::Direction::SOUTH) return -1;

        // Handle north / south walls first.
        if (dir == types::Direction::SOUTH)
            return y * w + x;
        if (dir == types::Direction::NORTH)
            return (y - 1) * w + x;

        // Now handle east / west walls.
        const auto offset = w * (h - 1);
        if (dir == types::Direction::EAST)
            return x * h + y + offset;
        if (dir == types::Direction::WEST)
            return (x - 1) * h + y + offset;

        // Make the compiler happy.
        throw std::invalid_argument("Trying to rank illegal direction.");
    }

#ifdef DEBUG
    void Maze::test_rankPositionS(const spelunker::types::Dimensions2D &dim) {
        std::set<int> ranks;
        const auto numwalls = calculateNumWalls(dim);
        const auto [width, height] = dim.values();

        for (auto x = 0; x < width; ++x)
            for (auto y = 0; y < height; ++y) {
                for (auto dir: types::directions()) {
                    const auto r = rankPositionS(dim, x, y, dir);
                    if (r != -1) ranks.insert(r);
                }
                if (y > 0 && y < height - 1)
                    assert(rankPositionS(dim, x, y, types::Direction::NORTH) == rankPositionS(dim, x, y - 1, types::Direction::SOUTH));
                if (x > 0 && x < width - 1)
                    assert(rankPositionS(dim, x - 1, y, types::Direction::EAST) == rankPositionS(dim, x, y, types::Direction::WEST));
            }

        for (auto i = 0; i < numwalls; ++i)
            assert(ranks.find(i) != ranks.end());
    }
#endif
}