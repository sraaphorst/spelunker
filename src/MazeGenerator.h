/**
 * MazeGenerator.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * The superclass of all maze generators.
 */

#ifndef SPELUNKER_MAZEGENERATOR_H
#define SPELUNKER_MAZEGENERATOR_H

#include "MazeAttributes.h"

namespace vorpal::maze {
    class Maze;

    class MazeGenerator {
    protected:
        const int width;
        const int height;

        /// The total number of possible interior (non-boundary) walls.
        const int numWalls;

    public:
        MazeGenerator(int w, int h);

        virtual ~MazeGenerator() = default;

        virtual const Maze generate() = 0;

    protected:
        /// Create an "empty layout", either full of walls, or without any walls (other than the boundary walls).
        /**
         * Initialize the layout of the maze to the "empty" layout.
         * This consists of the wall incidences all being set to true or false.
         * The boundary walls are always maintained.
         * @param walls indicates whether the maze should be all walls or no walls (except the boundary wall).
         * @return an "empty" layout
         */
        types::WallIncidence initializeEmptyLayout(bool walls);

        /// Create a map to reverse rankPosition: determine the two Positions on either side of a wall.
        const types::UnrankWallMap createUnrankWallMap() const;

        /// A static function used by unrankWallID, separated out for testing.
        static const types::UnrankWallMap createUnrankWallMapS(int w, int h);

        /// A function that maps Position to the corresponding ID of the wall in the maze.
        const types::WallID rankPos(const types::Position &p) const;

#ifndef NDEBUG
        /// Static test case for the createUnrankWallMapS function.
        static void test_createUnrankWallMapS(int w, int h);
#endif
    };
};

#endif //SPELUNKER_MAZEGENERATOR_H
