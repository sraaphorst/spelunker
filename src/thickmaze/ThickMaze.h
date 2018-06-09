/**
 * ThickMaze.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * ThickMaze represents a maze where the walls have thickness, i.e. take up a grid square.
 */

#pragma once

#include <sstream>
#include <string>

#include "typeclasses/Show.h"
#include "ThickMazeAttributes.h"
#include "StringThickMazeRenderer.h"

namespace spelunker::thickmaze {
    /**
     * Whereas @see{Maze} represents a maze where the walls are zero thickness, ThickMaze represents a maze
     * where walls take up a grid space.
     *
     * There is a monomorphism (but not surjective) from:
     * 1. Mazes of width w and height h to
     * 2. ThickMazes of width 2w-1 and height 2h-1.
     *
     * Thus, all of the @see{MazeGenerator}s can be used to make a subset of ThickMazes, i.e. ThickMazes of
     * odd width and height where walls are all contiguous cells of odd length >= 3. The mapping to this subset
     * is surjective, and thus Mazes and ThickMazes with this property are isomorphic.
     */
    class ThickMaze final {
    public:
        /**
         * Create a ThickMaze with the given width, height, and contents.
         * This class is effectively immutable.
         * @param w the width of the maze, minus boundary walls
         * @param h the height of the maze, minus boundary walls
         * @param c the contents of the maze, minus boundary walls
         */
        ThickMaze(int w, int h, const CellContents &c);
        virtual ~ThickMaze() = default;

        inline const int getWidth() const noexcept { return width; }
        inline const int getHeight() const noexcept { return height; }

        const CellType cellIs(int x, int y) const;

        /// Determine the number of walls a cell has.
        int numCellWalls(const types::Cell &c) const;

        /**
         * This algorithm swaps walls and floors, not including the border wall. It is useful for some
         * cellular automata algorithms, which are sometimes more wall-connected than floor-connected.
         * @return the reverse of the original maze, with walls and floors swapper.
         */
        const ThickMaze reverse() const;

        /// Make a ThickMaze into a braid maze, clearing dead ends with the given probability.
        /**
         * A braid maze is a maze with no dead ends: it will necessarily contain loops, and thus not be perfect.
         *
         * This method takes the current maze, and begins by creating a list of all the dead ends.
         * The list is then shuffled, and one-by-one, the algorithm considers each dead end cell and does the following:
         * 1. Determine if it is still indeed a dead end cell, as one of its walls may have already been removed.
         * 2. Use probability to determine if we apply braiding to this cell.
         * 3. Amongst the one-to-three neighbouring wall cells of this cell, pick one of the neighbours that
         *    eliminates more dead ends than any other. If multiple cells with the same count exist, pick randomly.
         * 4. Delete the wall cell.
         *
         * The reason for step (3) is because this allows us to minimize wall reduction, attempting to possibly
         * fix two dead ends at the same time (this cell, and the neighbour cell).
         *
         * @param probability the probability of fixing a given dead end
         * @return a new maze with walls removed to decrease the number of dead ends
         */
        const ThickMaze braid(double probability = 1.0) const;

        /// Find the dead ends for this maze.
        /**
         * Find a collection of all the dead ends for this maze.
         * A cell is considered a "dead end" if it has exactly three walls.
         * @return a collection of the dead end cells
         */
        const types::CellCollection findDeadEnds() const noexcept;

    private:
        /// Determine the number of walls a cell has for an instance of Contents.
        /**
         * Determine the number of walls a cell has for an instance of Contents.
         * We allow Contents to be supplied for operations like {@see braid}, where we want to
         * be working on an intermediate new CellContents.
         * @param c the cell of interest
         * @param cc the CellContents to use in counting
         * @return the number of cell walls of c in cc
         */
        int numCellWallsInContents(const types::Cell &c, const CellContents &cc) const;

        /**
         * Check a cell to make sure it appears in a valid place.
         * @param c the cell to check
         */
        void checkCell(const types::Cell &c) const;

        const int width;
        const int height;
        const CellContents contents;
    };
}

namespace spelunker::typeclasses {
    template<>
    struct Show<thickmaze::ThickMaze> {
        static std::string show(const thickmaze::ThickMaze &tm) {
            std::ostringstream out;
            thickmaze::StringThickMazeRenderer r(out);
            r.render(tm);
            return out.str();
        }
    };
}
