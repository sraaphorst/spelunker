/**
 * Maze.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#ifndef SPELUNKER_MAZE_H
#define SPELUNKER_MAZE_H

#include <algorithm>
#include <functional>
#include <optional>
#include <sstream>

#include "StringMazeRenderer.h"
#include "MazeAttributes.h"
#include "Show.h"

namespace spelunker::maze {
    // Class forward.
    class MazeGenerator;

    /// An immutable 2D planar maze.
    /**
     * A immutable 2D planar maze where walls have no thickness.
     * It encompasses a layout, as well as starting and ending positions.
     *
     * Here is a brief description as to how the layout is stored:
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
    class Maze final {
    public:
        /// Create a maze bounded by width and height, with a start and ending positions.
        /**
         * Creates a maze within a bounded rectangle of width and height, along with the start position and
         * the ending positions.
         * @param w width of the maze
         * @param h height of the maze
         * @param s an optional starting position
         * @param ends a collection of the ending positions
         * @param walls the wall incidence as described above
         */
        Maze(int w,
             int h,
             const types::PossibleStartCell &s,
             const types::CellCollection &ends,
             const types::WallIncidence &walls);

        /// Creates a maze bounded by width and height, but with no start / end positions.
        /**
         * Creates a maze within a bounded rectangle of width and height.
         * @param w width of the maze
         * @param h height of the maze
         * @param the wall incidence structure
         */
        Maze(int w,
             int h,
             const types::WallIncidence &walls);

        virtual ~Maze() = default;

        inline const int getWidth() const noexcept { return width; }
        inline const int getHeight() const noexcept { return height; }

        inline types::PossibleStartCell getStartingCell() const noexcept { return startCell; }
        inline types::CellCollection getEndingCells() const noexcept { return endingCells; }

        /// Create a new maze with the specified starting cell.
        const Maze withStartingCell(const types::PossibleStartCell &s) const;

        /// Create a new maze with the specified ending cells.
        const Maze withEndingCells(const types::CellCollection &ends) const;

        /// For a given position, determine if there is a wall.
        bool wall(const types::Position &p) const noexcept;

        /// For a given set of coordinates (x,y) and a direction, determine if there is a wall.
        bool wall(int x, int y, types::Direction d) const noexcept;

        /// Determine if two mazes are equal.
        bool operator==(const Maze &other) const;

        /// Determine if two mazes are not equal.
        bool operator!=(const Maze &other) const {
            return !(*this == other);
        }

        /// Apply a symmetry to this maze to get a new one.
        const Maze applySymmetry(types::Symmetry s) const;

    private:
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
        types::WallID rankPosition(const types::Position &p) const;

        /// A function that maps a cell (x,y) and direction to wall ranks.
        types::WallID rankPosition(int x, int y, types::Direction d) const;

        /// A static function used by rankPosition, separated out for testing.
        static types::WallID rankPositionS(int w, int h, int x, int y, types::Direction d);

        /// Check the start and end positions to make sure they appear in valid places.
        void checkCells() const;

        /**
         * Check a cell to make sure it appears in a valid place.
         * @param c the cell to check
         */
        void checkCell(const types::Cell &c) const;

        /// Static auxiliary method to check to see if a cell is valid.
        static void checkCell(int w, int h, int x, int y);

        const int width;
        const int height;
        const int numWalls;
        const types::PossibleStartCell startCell;
        const types::CellCollection endingCells;
        const types::WallIncidence wallIncidence;

#ifndef NDEBUG
    public:
        /// Static test case for the rankPositionS function.
        static void test_rankPositionS(int w, int h);
#endif

        friend MazeGenerator;
    };
}

namespace spelunker::typeclasses {
    template<>
    struct Show<maze::Maze> {
        static std::string show(const maze::Maze &m) {
            std::ostringstream out;
            maze::StringMazeRenderer r(out);
            r.render(m);
            return out.str();
        }

        static constexpr bool is_instance = true;
        using type = maze::Maze;
    };
}
#endif //SPELUNKER_MAZE_H
