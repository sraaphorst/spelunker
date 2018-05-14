/**
 * Maze.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#ifndef SPELUNKER_MAZE_H
#define SPELUNKER_MAZE_H

#include <algorithm>
#include "MazeAttributes.h"

namespace vorpal {
    namespace maze {
        /// The most generic type of planar 2D maze.
        /**
         * The most generic characteristics of a 2D planar maze.
         * It encompasses a layout, as well as starting and ending positions.
         *
         * If we have width w and height h, and we assume that the maze is surrounded by walls,
         * then we have:
         * 1. c = w * h cells; and
         * 2. z = (w-1)*h + w*(h-1) walls.
         *
         * Here is an example of a 4x3 maze. The bounding walls are not shown:
         *
         *  0 1 2 3
         * 0 | | |
         *  -+-+-+-
         * 1 | | |
         *  -+-+-+-
         * 2 | | |
         *
         * The maze can be fully represented by the incidence of walls. We assign a rank to the walls in the
         * range [0,z), and then represent their incidence by a vector of bool. For convenience of working with
         * cells, we have a function that takes a cell coordinates and a direction and returns the rank of the
         * wall, or -1 if the wall is a bounding wall.
         */
        class Maze {
        public:
            /// Create a completely empty maze bounded by width and height.
            /**
             * Creates a completely empty maze within a bounded rectangle of width
             * and height, along with the start position and the ending positions.
             * @param w width of the maze
             * @param h height of the maze
             * @param s the starting position
             * @param ends the ending positions
             */
            Maze(const int w,
                 const int h,
                 const Cell s,
                 const CellCollection ends);
            virtual ~Maze() = default;

            static inline Cell cell(int x, int y) { return std::make_pair(x,y); }
            static inline Position pos(int x, int y, Direction d) { return std::make_pair(cell(x,y),d); }
            static inline Position pos(const Cell &c, Direction d) { return std::make_pair(c,d); }

            virtual inline const WallIncidence &getWallIncidence() noexcept final { return wallIncidence; }

            virtual inline int getWidth() noexcept final { return width; }
            virtual inline int getHeight() noexcept final { return height; }

            virtual inline Cell getStartingCell() noexcept final { return startCell; }
            virtual inline CellCollection getEndngCells() noexcept final { return endingCells; }

            /**
             * Generate a maze that meets the requirements.
             */
            virtual const WallIncidence generate() { WallIncidence(); };
        protected:
            /// Initialize the layout of the maze to the "empty" layout.
            /**
             * Initialize the layout of the maze to the "empty" layout.
             * @param walls indicates whether the maze should be all walls or no walls (except the boundary wall).
             */
            WallIncidence initializeEmptyLayout(bool walls);

            /// A function that maps positions to wall ranks.
            /**
             * A position is a current location in the maze, which corresponds to a cell (x,y) and the direction
             * d in which one is facing. This function takes a position and finds the wall rank to which it
             * corresponds, or -1 if it corresponds to a bounding wall, i.e. a wall at the boundary of the maze.
             * @param x the x coordinate
             * @param y the y coordinate
             * @param d the direction
             * @return the rank of the wall at this position
             */
            virtual WallID rankPosition(const Position &p) final;

            /// A static function used by rankPosition, separated out for testing.
            static WallID rankPositionS(const int w, const int h, const int x, const int y, const Direction d);

            /// Create a map to reverse rankPosition: determine the two Positions on either side of a wall.
            virtual const UnrankWallMap createUnrankWallMap();

            /// A static function used by unrankWallID, separated out for testing.
            static const UnrankWallMap createUnrankWallMapS(const int w, const int h);


            // We consider these "fixed characteristics" of the maze.
            const int width;
            const int height;
            const Cell startCell;
            const CellCollection endingCells;
            const int numWalls;
            const WallIncidence wallIncidence;

        private:
            /// Check the start and end positions to make sure they appear in valid places.
            void checkCells();

            /**
             * Check a position to make sure it appears in a valid place.
             * @param p the position to check
             */
            void checkCell(const Cell &c);

#ifndef NDEBUG
            /// Static test case for the rankPositionS function.
            static void test_rankPositionS(const int w, const int h);

            /// Static test case for the createUnrankWallMapS function.
            static void test_createUnrankWallMapS(const int w, const int h);
#endif
        };
    }
}
#endif //SPELUNKER_MAZE_H
