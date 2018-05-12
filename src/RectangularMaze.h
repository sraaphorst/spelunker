/**
 * Maze.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#ifndef SPELUNKER_MAZE_H
#define SPELUNKER_MAZE_H

#include <set>
#include <tuple>
#include <vector>
#include "MazeAttributes.h"

namespace vorpal {
        namespace maze {
            class RectangularMaze {
            public:
                /// Initializes a rectangular maze of width x height.
                /**
                 * Initializes the boundary of a rectangular maze of width x height, with the
                 * start position in the upper left corner and the end position in the lower right
                 * corner
                 * @param width the width of the maze
                 * @param height the height of the maze
                 */
                RectangularMaze(int width, int height);

                /// Initializes a rectangular maze of width x height with a set entrace and one or more exits.
                /**
                 * Initializes the boundary of a rectangular maze of weigth x height, with the
                 * start position and end positions as specified.
                 * @param width the width of the maze
                 * @param height the height of the meze
                 * @param startPosition the starting position
                 * @param endPositions a set of possible ending positions.
                 */
                RectangularMaze(int width, int height, Position &startPosition, std::vector< Position> endPositions);
                virtual ~RectangularMaze() = default;

                virtual const Layout &getLayout() = 0;
                virtual const Position &getStartPosition() = 0;
                virtual const std::vector< Position > getEndPositions() = 0;

            private:
                void createBorder();
            };
        };
};
#endif //SPELUNKER_MAZE_H
