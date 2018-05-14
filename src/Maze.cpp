/**
 * Maze.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <cassert>
#include <map>
#include <tuple>
#include <vector>
#include "Exceptions.h"
#include "MazeAttributes.h"
#include "Maze.h"

namespace vorpal::maze {
    Maze::Maze(const int w,
               const int h,
               const Cell s,
               const CellCollection ends)
            : width(w), height(h), startCell(s), endingCells(ends), numWalls((w - 1) * h + w * (h - 1)),
              wallIncidence(generate()) {
        checkCells();
    }

    WallIncidence Maze::initializeEmptyLayout(bool walls) {
        // This involves just setting the wall incidences to true or false.
        return WallIncidence(numWalls, walls);
    }

    WallID Maze::rankPosition(const Position &p) {
        const auto &cell = p.first;
        checkCell(cell);

        const auto x = cell.first;
        const auto y = cell.second;
        const auto d = p.second;

        return rankPositionS(width, height, x, y, d);
    };

    WallID Maze::rankPositionS(const int width,
                               const int height,
                               const int x,
                               const int y,
                               const vorpal::maze::Direction d) {
        //  Get rid of all the boundary cases first to simplify, as these are easy to identify.
        if (x == 0 && d == WEST) return -1;
        if (x == (width - 1) && d == EAST) return -1;
        if (y == 0 && d == NORTH) return -1;
        if (y == (height - 1) && d == SOUTH) return -1;

        // Handle north / south walls first.
        if (d == SOUTH)
            return y * width + x;
        if (d == NORTH)
            return (y - 1) * width + x;

        // Now handle east / west walls.
        const auto offset = width * (height - 1);
        if (d == EAST)
            return x * height + y + offset;
        if (d == WEST)
            return (x - 1) * height + y + offset;

    }

    const UnrankWallMap Maze::createUnrankWallMap() {
        return createUnrankWallMapS(width, height);
    }

    const UnrankWallMap Maze::createUnrankWallMapS(const int w, const int h) {
        UnrankWallMap umap;

        // The easiest way to do this is the inefficient way: iterate over all possible positions and collect
        // up their wall ranks. We need a vector to do this, which we will turn into a pair since there should
        // be two or a failure has occurred.
        std::map<WallID, std::vector<Position> > unrankings;
        for (auto x = 0; x < w; ++x)
            for (auto y = 0; y < h; ++y) {
                for (auto d : Directions) {
                    unrankings[rankPositionS(w, h, x, y, d)].emplace_back(pos(x, y, d));
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

    void Maze::checkCells() {
        checkCell(startCell);
        for (auto p: endingCells)
            checkCell(p);
    }

    void Maze::checkCell(const Cell &c) {
        if (c.first < 0 || c.first > width || c.second < 0 || c.second >= height)
            throw OutOfBoundsCell(c);
    }

#ifndef NDEBUG
    void Maze::test_rankPositionS(const int width, const int height) {
        std::set<int> ranks;
        const auto numwalls = (width - 1) * height + width * (height - 1);

        for (auto x = 0; x < width; ++x)
            for (auto y = 0; y < height; ++y) {
                for (auto d: Directions) {
                    const auto r = rankPositionS(width, height, x, y, d);
                    if (r != -1) ranks.insert(r);
                }
                if (y > 0 && y < height - 1)
                    assert(rankPositionS(width, height, x, y, NORTH) == rankPositionS(width, height, x, y - 1, SOUTH));
                if (x > 0 && x < width - 1)
                    assert(rankPositionS(width, height, x - 1, y, EAST) == rankPositionS(width, height, x, y, WEST));
            }

        for (auto i = 0; i < numwalls; ++i)
            assert(ranks.find(i) != ranks.end());
    }

    void Maze::test_createUnrankWallMapS(const int width, const int height) {
        const auto m = createUnrankWallMapS(width, height);
        for (auto kv : m) {
            const auto &rk = kv.first;

            const auto &p1 = kv.second.first;
            const auto &x1 = p1.first.first;
            const auto &y1 = p1.first.second;
            const auto &d1 = p1.second;
            assert(rankPositionS(width, height, x1, y1, d1) == rk);

            const auto &p2 = kv.second.second;
            const auto &x2 = p2.first.first;
            const auto &y2 = p2.first.second;
            const auto &d2 = p2.second;
            assert(rankPositionS(width, height, x1, y1, d1) == rk);
        }
    }
#endif
}