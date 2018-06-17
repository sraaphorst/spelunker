/**
 * MazeGenerator.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <cassert>

#include <types/CommonMazeAttributes.h>
#include <types/Direction.h>
#include "Maze.h"
#include "MazeAttributes.h"
#include "MazeGenerator.h"

namespace spelunker::maze {
    MazeGenerator::MazeGenerator(const types::Dimensions2D &d)
        : AbstractMazeGenerator{d}, numWalls{calculateNumWalls(d)} {}

    MazeGenerator::MazeGenerator(const int w, const int h)
        : MazeGenerator{types::Dimensions2D{w, h}} {}

    const UnrankWallMap MazeGenerator::createUnrankWallMap() const noexcept {
        return createUnrankWallMapS(getDimensions());
    }

    const UnrankWallMap MazeGenerator::createUnrankWallMapS(const types::Dimensions2D &dim) noexcept {
        UnrankWallMap umap;

        // The easiest way to do this is the inefficient way: iterate over all possible positions and collect
        // up their wall ranks. We need a vector to do this, which we will turn into a pair since there should
        // be two or a failure has occurred.
        const auto [width, height] = dim.values();
        std::map<WallID, std::vector<types::Position>> unrankings;
        for (auto x = 0; x < width; ++x)
            for (auto y = 0; y < height; ++y) {
                for (const auto dir : types::directions()) {
                    const auto rk = Maze::rankPositionS(dim, x, y, dir);
                    if (rk != -1)
                        unrankings[rk].emplace_back(pos(x, y, dir));
                }
            }

        // Now convert to pairs.
        for (const auto &kv : unrankings) {
            const auto &[rk, ps] = kv;
            assert(ps.size() == 2);
            umap.insert(std::make_pair(rk, std::make_pair(ps[0], ps[1])));
        }

        return umap;
    }

    const WallID MazeGenerator::rankPos(const types::Position &p) const {
        const auto [c,d] = p;
        const auto [cx, cy] = c;
        return Maze::rankPositionS(getDimensions(), cx, cy, d);
    }

    const types::Neighbours MazeGenerator::unvisitedNeighbours(const types::Cell &c,
                                                               const types::CellIndicator &ci) const {
        return neighbours(c, [&ci](const int x, const int y) { return !ci[x][y]; });
    }

    const types::Neighbours MazeGenerator::visitedNeighbours(const types::Cell &c,
                                                             const types::CellIndicator &ci) const {
        return neighbours(c, [&ci](const int x, const int y) { return ci[x][y]; });
    }

    const types::Neighbours MazeGenerator::allNeighbours(const types::Cell &c) const {
        return neighbours(c, [](const int, const int) { return true; });
    }

    const types::Neighbours MazeGenerator::neighbours(const types::Cell &c,
                                                      std::function<bool(const int, const int)> filter) const {
        getDimensions().checkCell(c);

        types::Neighbours nbrs;
        const auto [x, y] = c;
        const auto [width, height] = getDimensions().values();

        // Note we have to SWITCH the directions here because we want the direction in which we come INTO that node.
        if (x - 1 >= 0 && filter(x-1,y))
            nbrs.emplace_back(types::pos(x - 1, y, types::Direction::EAST));
        if (y - 1 >= 0 && filter(x,y-1))
            nbrs.emplace_back(types::pos(x, y - 1, types::Direction::SOUTH));
        if (x + 1 < width && filter(x+1,y))
            nbrs.emplace_back(types::pos(x + 1, y, types::Direction::WEST));
        if (y + 1 < height && filter(x,y+1))
            nbrs.emplace_back(types::pos(x, y + 1, types::Direction::NORTH));
        return nbrs;
    }

#ifdef DEBUG
    void MazeGenerator::test_createUnrankWallMapS(const types::Dimensions2D &dim) {
        const auto m = createUnrankWallMapS(dim);
        for (const auto &kv : m) {
            // kv is std::map<WallID, std::pair<types::Position, types::Position> >
            const auto &[rk, ps] = kv;
            const auto &[p1, p2] = ps;

            const auto &[c1, d1] = p1;
            const auto [x1, y1] = c1;
            assert(Maze::rankPositionS(dim, x1, y1, d1) == rk);

            const auto &[c2, d2] = p2;
            const auto [x2, y2] = c2;
            assert(Maze::rankPositionS(dim, x2, y2, d2) == rk);
        }
    }
#endif
}