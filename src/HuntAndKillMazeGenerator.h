/**
 * HuntAndKillMazeGenerator.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#ifndef SPELUNKER_HUNTANDKILLMAZEGENERATOR_H
#define SPELUNKER_HUNTANDKILLMAZEGENERATOR_H

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
        HuntAndKillMazeGenerator(int w, int h);
        ~HuntAndKillMazeGenerator() final = default;

        const Maze generate() final;

    private:
        void randomPathCarving(int startX, int startY, CellIndicator &ci, WallIncidence &wi) const noexcept;
    };
};

#endif //SPELUNKER_HUNTANDKILLMAZEGENERATOR_H
