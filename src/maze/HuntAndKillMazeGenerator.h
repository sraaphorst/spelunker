/**
 * HuntAndKillMazeGenerator.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <types/CommonMazeAttributes.h>
#include <types/Dimensions2D.h>

#include "MazeAttributes.h"
#include "MazeGenerator.h"

namespace spelunker::maze {
    class Maze;

    /**
     * The Hunt-and-kill algorithm picks a random spot in a maze and marks it as visited. It then proceeds to
     * do a random walk through unvisited nodes until it is no longer able to do so, at which point, it carves
     * a path.
     *
     * It then enters into hunt mode, where it scans the maze - usually in an ordered way - to find an unvisited
     * node adjacent to a node, at which point, it carves a passage between them and begins the random walk
     * with the formerly uncovered node.
     *
     * When no such node can be found, we are done and the result is a perfect maze.
     */
    class HuntAndKillMazeGenerator final : public MazeGenerator {
    public:
        HuntAndKillMazeGenerator(const types::Dimensions2D &d);
        HuntAndKillMazeGenerator(int w, int h);
        ~HuntAndKillMazeGenerator() final = default;

        const Maze generate() const noexcept final;

    private:
        void randomPathCarving(int startX, int startY, types::CellIndicator &ci, WallIncidence &wi) const noexcept;
    };
};

