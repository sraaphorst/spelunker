/**
 * MazeGenerator.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <cassert>

#include "Maze.h"
#include "MazeAttributes.h"
#include "MazeGenerator.h"

namespace spelunker::maze {
    MazeGenerator::MazeGenerator(const int w, const int h)
            : width(w), height(h), numWalls(calculateNumWalls(w, h)) {}

    WallIncidence MazeGenerator::initializeEmptyLayout(bool walls) {
        // This involves just setting the wall incidences to true or false.
        return WallIncidence(numWalls, walls);
    }

    const UnrankWallMap MazeGenerator::createUnrankWallMap() const {
        return createUnrankWallMapS(width, height);
    }

    const UnrankWallMap MazeGenerator::createUnrankWallMapS(const int w, const int h) {
        UnrankWallMap umap;

        // The easiest way to do this is the inefficient way: iterate over all possible positions and collect
        // up their wall ranks. We need a vector to do this, which we will turn into a pair since there should
        // be two or a failure has occurred.
        std::map<WallID, std::vector<Position> > unrankings;
        for (auto x = 0; x < w; ++x)
            for (auto y = 0; y < h; ++y) {
                for (auto d : directions()) {
                    const int rk = Maze::rankPositionS(w, h, x, y, d);
                    if (rk != -1)
                        unrankings[rk].emplace_back(pos(x, y, d));
                }
            }

        // Now convert to pairs.
        for (auto kv : unrankings) {
            const auto rk = kv.first;
            const auto ps = kv.second;

            assert(ps.size() == 2);
            umap.insert(std::make_pair(rk, std::make_pair(ps[0], ps[1])));
        }

        return umap;
    }

    const WallID MazeGenerator::rankPos(const Position &p) const {
        return Maze::rankPositionS(width, height, p.first.first, p.first.second, p.second);
    }

    const Neighbours MazeGenerator::unvisitedNeighbours(const Cell &c,
                                                               const CellIndicator &ci) const {
        return neighbours(c, [&ci](const int x, const int y) { return !ci[x][y]; });
    }

    const Neighbours MazeGenerator::visitedNeighbours(const Cell &c,
                                                             const CellIndicator &ci) const {
        return neighbours(c, [&ci](const int x, const int y) { return ci[x][y]; });
    }

    const Neighbours MazeGenerator::allNeighbours(const Cell &c) const {
        return neighbours(c, [](const int, const int) { return true; });
    }

    const Neighbours MazeGenerator::neighbours(const Cell &c,
                                                      std::function<bool(const int, const int)> filter) const {
        Neighbours nbrs;

        const auto [x, y] = c;

        // Note we have to SWITCH the directions here because we want the direction in which we come INTO that node.
        if (x - 1 >= 0 && filter(x-1,y))
            nbrs.emplace_back(pos(x - 1, y, EAST));
        if (y - 1 >= 0 && filter(x,y-1))
            nbrs.emplace_back(pos(x, y - 1, SOUTH));
        if (x + 1 < width && filter(x+1,y))
            nbrs.emplace_back(pos(x + 1, y, WEST));
        if (y + 1 < height && filter(x,y+1))
            nbrs.emplace_back(pos(x, y + 1, NORTH));
        return nbrs;
    }

#ifndef NDEBUG
    void MazeGenerator::test_createUnrankWallMapS(const int w, const int h) {
        const auto m = createUnrankWallMapS(w, h);
        for (auto kv : m) {
            const auto &rk = kv.first;

            const auto &p1 = kv.second.first;
            const auto &x1 = p1.first.first;
            const auto &y1 = p1.first.second;
            const auto &d1 = p1.second;
            assert(Maze::rankPositionS(w, h, x1, y1, d1) == rk);

            const auto &p2 = kv.second.second;
            const auto &x2 = p2.first.first;
            const auto &y2 = p2.first.second;
            const auto &d2 = p2.second;
            assert(Maze::rankPositionS(w, h, x1, y1, d1) == rk);
        }
    }
#endif
}