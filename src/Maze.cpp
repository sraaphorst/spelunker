/**
 * Maze.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <cassert>
#include "Exceptions.h"
#include "MazeAttributes.h"
#include "Maze.h"

namespace vorpal {
    namespace maze {
        Maze::Maze(const int w,
                   const int h,
                   const Cell s,
                   const CellCollection ends)
        : width(w), height(h), startCell(s), endingCells(ends), numWalls((w-1)*h + w*(h-1)), wallIncidence(generate()) {}

        WallIncidence Maze::initializeEmptyLayout(bool walls) {
            // This involves just setting the wall incidences to true or false.
            return WallIncidence(numWalls, walls);
        }

        WallID Maze::rankPosition(const Position &p) {
            const Cell &cell = p.first;
            checkCell(cell);

            const int x = cell.first;
            const int y = cell.second;
            const Direction d = p.second;

            return rankPositionS(width, height, x, y, d);
        };

        WallID Maze::rankPositionS(const int width,
                                   const int height,
                                   const int x,
                                   const int y,
                                   const vorpal::maze::Direction d) {
            //  Get rid of all the boundary cases first to simplify, as these are easy to identify.
            if (x == 0 && d == WEST)         return -1;
            if (x == (width-1) && d == EAST) return -1;
            if (y == 0 && d == NORTH)        return -1;
            if (y == (height-1) && d == SOUTH)        return -1;

            // Handle north / south walls first.
            if (d == SOUTH)
                return y * width + x;
            if (d == NORTH)
                return (y-1) * width + x;

            // Now handle east / west walls.
            const int offset = width * (height - 1);
            if (d == EAST)
                return x * height + y + offset;
            if (d == WEST)
                return (x-1) * height + y + offset;

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
            std::set< int > ranks;
            int numwalls = (width-1) * height + width * (height-1);

            for (auto x=0; x < width; ++x)
                for (auto y=0; y < height; ++y) {
                    for (auto d: Directions) {
                        int r = rankPositionS(width, height, x, y, d);
                        if (r != -1) ranks.insert(r);
                    }
                    if (y > 0 && y < height-1)
                        assert(rankPositionS(width, height, x, y, NORTH) == rankPositionS(width, height, x, y-1, SOUTH));
                    if (x > 0 && x < width-1)
                        assert(rankPositionS(width, height, x-1, y, EAST) == rankPositionS(width, height, x, y, WEST));
                }

            for (int i=0; i < numwalls; ++i)
                assert(ranks.find(i) != ranks.end());
        }
    }
#endif
}