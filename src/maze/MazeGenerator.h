/**
 * MazeGenerator.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * The superclass of all maze generators.
 */

#pragma once

#include <functional>

#include "types/CommonMazeAttributes.h"
#include "MazeAttributes.h"

namespace spelunker::maze {
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
        WallIncidence initializeEmptyLayout(bool walls);

        /// Create a map to reverse rankPosition: determine the two Positions on either side of a wall.
        const UnrankWallMap createUnrankWallMap() const;

        /// A static function used by unrankWallID, separated out for testing.
        static const UnrankWallMap createUnrankWallMapS(int w, int h);

        /// A function that maps Position to the corresponding ID of the wall in the maze.
        const WallID rankPos(const types::Position &p) const;

        /// Rank a cell.
        const inline int rankCell(const int x, const int y) { return y * width + x; }

        /// Unrank a cell.
        const inline types::Cell unrankCell(const int rk) { return types::cell(rk % width, rk / width); }

        /// Find all of the unvisited neighbours of a cell.
        const types::Neighbours unvisitedNeighbours(const types::Cell &c, const types::CellIndicator &ci) const;

        /// Find all of the visited neighbours of a cell.
        const types::Neighbours visitedNeighbours(const types::Cell &c, const types::CellIndicator &ci) const;

        /// Find all of the valid neighbours of a cell.
        const types::Neighbours allNeighbours(const types::Cell &c) const;

    private:
        /// Auxiliary helper function for generating lists of neighbours.
        /**
         * Auxiliary helper function for generating lists of neightbours.
         * The filter determines what neighbours are placed on the list.
         * @param c the cell whose neighbours we list
         * @param filter a filter to filter the neighbours
         * @return a list of the neighbours of c where the filter is true
         */
        const types::Neighbours neighbours(const types::Cell &c,
                                           std::function<bool(const int, const int)> filter) const;

#ifndef NDEBUG
    public:
        /// Static test case for the createUnrankWallMapS function.
        static void test_createUnrankWallMapS(int w, int h);
#endif
    };
};

