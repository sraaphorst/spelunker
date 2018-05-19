/**
 * MazeGenerator.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <cassert>

#include "Maze.h"
#include "MazeAttributes.h"
#include "MazeGenerator.h"

namespace vorpal::maze {
    MazeGenerator::MazeGenerator(const int w, const int h)
            : width(w), height(h), numWalls(types::calculateNumWalls(w, h)) {}

    types::WallIncidence MazeGenerator::initializeEmptyLayout(bool walls) {
        // This involves just setting the wall incidences to true or false.
        return types::WallIncidence(numWalls, walls);
    }

    const types::UnrankWallMap MazeGenerator::createUnrankWallMap() const {
        return createUnrankWallMapS(width, height);
    }

    const types::UnrankWallMap MazeGenerator::createUnrankWallMapS(const int w, const int h) {
        types::UnrankWallMap umap;

        // The easiest way to do this is the inefficient way: iterate over all possible positions and collect
        // up their wall ranks. We need a vector to do this, which we will turn into a pair since there should
        // be two or a failure has occurred.
        std::map<types::WallID, std::vector<types::Position> > unrankings;
        for (auto x = 0; x < w; ++x)
            for (auto y = 0; y < h; ++y) {
                for (auto d : types::directions()) {
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