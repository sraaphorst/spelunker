/**
 * Maze.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#ifndef SPELUNKER_MAZE_H
#define SPELUNKER_MAZE_H

#include "MazeAttributes.h"

namespace vorpal {
    namespace maze {
        /// The most generic type of planar 2D maze.
        /**
         * The most generic characteristics of a 2D planar maze.
         * It encompasses a layout, as well as starting and ending positions.
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
            Maze(const int w, const int h, const Position s, const PositionCollection ends);
            virtual ~Maze() = default;

            virtual inline Layout &getLayout() noexcept final { return layout; }
            virtual inline void setLayout(const Layout &playout) final;

            virtual inline int getWidth() noexcept final { return width; }
            virtual inline int getHeight() noexcept final { return height; }

            virtual inline Position getStartingPosition() noexcept final { return start; }
            virtual inline PositionCollection getEndngPositions() noexcept final { return endingPositions; }

            /**
             * Generate a maze that meets the requirements.
             */
            virtual void generate() = 0;
        protected:
            /// Initialize the layout of the maze to the "empty" layout.
            /**
             * Initialize the layout of the maze to the "empty" layout.
             * This is a width x height vector of all walls (if walls is true) or no walls (if walls is empty).
             * @param walls indicates whether the maze should be all walls or no walls
             */
            void initializeEmptyLayout(bool walls);

            int width;
            int height;
            Layout layout;
            Position start;
            PositionCollection endingPositions;

        private:
            /**
             * Check the start and end positions to make sure they appear in valid places.
             */
            void checkPositions();

            /**
             * Check a position to make sure it appears in a valid place.
             * @param p the position to check
             */
            void checkPosition(const Position &p);
        };
    }
}
#endif //SPELUNKER_MAZE_H
