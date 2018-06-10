/**
 * Maze.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <iostream>
#include <algorithm>
#include <functional>
#include <optional>
#include <sstream>

#include "types/CommonMazeAttributes.h"
#include "typeclasses/Homomorphism.h"
#include "MazeAttributes.h"
#include "typeclasses/Show.h"
#include "StringMazeRenderer.h"
#include "thickmaze/ThickMaze.h"
#include "thickmaze/ThickMazeAttributes.h"

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
             const types::PossibleCell &s,
             const types::CellCollection &ends,
             const WallIncidence &walls);

        /// Creates a maze bounded by width and height, but with no start / end positions.
        /**
         * Creates a maze within a bounded rectangle of width and height.
         * @param w width of the maze
         * @param h height of the maze
         * @param the wall incidence structure
         */
        Maze(int w,
             int h,
             const WallIncidence &walls);

        ~Maze() = default;

        inline const int getWidth() const noexcept { return width; }
        inline const int getHeight() const noexcept { return height; }

        inline types::PossibleCell getStartingCell() const noexcept { return startCell; }
        inline types::CellCollection getEndingCells() const noexcept { return endingCells; }

        /// Create a new maze with the specified starting cell.
        const Maze withStartingCell(const types::PossibleCell &s) const;

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

        /// Determine the number of walls a cell has.
        int numCellWalls(const types::Cell &c) const;

        /// Apply a symmetry to this maze to get a new one.
        const Maze applySymmetry(types::Symmetry s) const;

        /// Make a perfect maze into a 2w x 2h unicursal maze (aka labyrinth).
        /**
         * Make a perfect maze of dimension w x h into a unicursal maze (aka labyrinth) of
         * dimension 2w x 2h.
         *
         * This is done by taking every call and splitting it into a 2 x 2 set of cells
         * where we bisect the cell, but with dead ends, leave one cell open at the end
         * of the dead end to allow traversal through and around it.
         *
         * Note: The entrance of the original maze, if it exists and is against a border, is
         * used to determine the entrance point / exit point of this maze.
         * Otherwise, this maze is just one huge loop.
         *
         * Note: Exit points of the original maze are ignored in the construction of this maze,
         * as by the construction to "unicursalize" this maze, the entrance and exit must be
         * next to one another.
         *
         * Idea for this algorithm inspired by:
         * https://github.com/jamis/theseus/blob/eff25e2d02da00bb9515690b83008ec05e852317/lib/theseus/orthogonal_maze.rb#L108
         *
         * @return a unicursal modification of this maze
         */
        const Maze makeUnicursal() const;

        /// Make a maze into a braid maze, clearing dead ends with the given probability.
        /**
         * A braid maze is a maze with no dead ends: it will necessarily contain loops, and thus not be perfect.
         *
         * This method takes the current maze, and begins by creating a list of all the dead ends.
         * The list is then shuffled, and one-by-one, the algorithm considers each dead end cell and does the following:
         * 1. Determine if it is still indeed a dead end cell, as one of its walls may have already been removed.
         * 2. Use probability to determine if we apply braiding to this cell.
         * 3. Amongst the three neighbouring cells sharing a wall with this cell, pick one of the neighbours that
         *    has the largest number of walls. Ties are broken randomly.
         * 4. Delete the wall between this cell and the selected neighbour.
         *
         * The reason for step (3) is because this allows us to minimize wall reduction, attempting to possibly
         * fix two dead ends at the same time (this cell, and the neighbour cell).
         *
         * @param probability the probability of fixing a given dead end
         * @return a new maze with walls removed to decrease the number of dead ends
         */
        const Maze braid(double probability = 1.0) const;

        /// Find the dead ends for this maze.
        /**
         * Find a collection of all the dead ends for this maze.
         * A cell is considered a "dead end" if it has exactly three walls.
         * @return a collection of the dead end cells
         */
        const types::CellCollection findDeadEnds() const noexcept;

    private:
        /// Determine the number of walls a cell has for an instance of WallIncidence.
        /**
         * Determine the number of walls a cell has for an instance of WallIncidence.
         * We allow a WallIncidence to be supplied for operations like {@see braid}, where we want to
         * be working on an intermediate new WallIncidence.
         * @param c the cell of interest
         * @param wi the WallIncidence to use in counting
         * @return the number of cell walls of c in wi
         */
        int numCellWallsInWI(const types::Cell &c, const WallIncidence &wi) const;

        /// Take a Position and map it to the neighbouring cell that it faces, or nothing if out of bounds.
        /**
         * Given a position, find the cell that the position is facing if one is in bounds.
         * If there is no such cell, i.e. the position faces the border, a value of none is returned.
         * @param p the position to evaluate
         * @return a value if the cell exists, and nothing otherwise
         */
        const types::PossibleCell evaluatePosition(const types::Position &p) const noexcept;

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
        WallID rankPosition(const types::Position &p) const;

        /// A function that maps a cell (x,y) and direction to wall ranks.
        WallID rankPosition(int x, int y, types::Direction d) const;

        /// A static function used by rankPosition, separated out for testing.
        static WallID rankPositionS(int w, int h, int x, int y, types::Direction d);

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
        const types::PossibleCell startCell;
        const types::CellCollection endingCells;
        const WallIncidence wallIncidence;

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

    template<>
    struct Homomorphism<maze::Maze, thickmaze::ThickMaze> {
        static const thickmaze::ThickMaze morph(const maze::Maze &m) {
            const int mwidth  = m.getWidth();
            const int mheight = m.getHeight();
            const int twidth  = 2 * mwidth - 1;
            const int theight = 2 * mheight - 1;

            auto contents = thickmaze::createThickMazeCellContents(twidth, theight);

            // Iterate over the walls of the maze and add them to the thick maze, focusing
            // on the east and the south walls of maze.
            // For each maze wall, mark (provided not out of bounds) three wall segments
            // in the thick maze.
            for (auto x = 0; x < mwidth; ++x)
                for (auto y = 0; y < mheight; ++y) {
                    // Ignore the last row of maze when adding southern walls.
                    if (y < mheight-1 && m.wall(x, y, SOUTH)) {
                        // Find the central position in the thick maze.
                        const int cx = 2 * x;
                        const int cy = 2 * y + 1;
                        if (cx > 0) contents[cx-1][cy] = thickmaze::WALL;
                        contents[cx][cy] = thickmaze::WALL;
                        if (cx < twidth-1) contents[cx+1][cy] = thickmaze::WALL;
                    }

                    // Ignore the last row of maze when adding eastern walls.
                    if (x < mwidth - 1 && m.wall(x, y, EAST)) {
                        // Find the central position in the thick maze.
                        const int cx = 2 * x + 1;
                        const int cy = 2 * y;
                        if (cy > 0) contents[cx][cy-1] = thickmaze::WALL;
                        contents[cx][cy] = thickmaze::WALL;
                        if (cy < theight-1) contents[cx][cy+1] = thickmaze::WALL;
                    }
                }
            return thickmaze::ThickMaze(twidth, theight, contents);
        }

        static constexpr bool is_instance = true;
        using src = maze::Maze;
        using type = thickmaze::ThickMaze;
    };
}

