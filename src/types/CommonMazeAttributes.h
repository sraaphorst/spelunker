/**
 * CommonMazeAttributes.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Attributes common to both regular and thick mazes.
 */

#pragma once

#include <optional>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

#include "Direction.h"

namespace spelunker::types {
    /// An (x,y) cell in a maze.
    using Cell = std::pair<int, int>;

    /// A collection of cells.
    using CellCollection = std::vector<Cell>;

    /// An indicator as to whether or not we've processed a Cell for a row.
    using CellRowIndicator = std::vector<bool>;

    /// An indicator as to whether or not we've processed a Cell.
    using CellIndicator = std::vector<CellRowIndicator>;

    /// Determine what direction two adjacent cells are apart.
    inline Direction cellDirection(const Cell &c1, const Cell &c2) {
        auto [x1, y1] = c1;
        auto [x2, y2] = c2;

        if (x1 - x2 == -1 && y1 == y2)
            return Direction::EAST;
        if (x1 - x2 ==  1 && y1 == y2)
            return Direction::WEST;
        if (y1 - y2 == -1 && x1 == x2)
            return Direction::SOUTH;
        if (y1 - y2 ==  1 && x1 == x2)
            return Direction::NORTH;

        // If we reach this point, the cells aren't adjacent.
        throw std::domain_error("Vertices are not adjacent");
    }

    /// A possible cell: it may be defined, or not.
    using PossibleCell = std::optional<Cell>;

    /// A position in a maze, i.e. a Cell and a Direction.
    using Position = std::pair<Cell, Direction>;

    /// The neighbours of a cell. Walls should face towards the original cell.
    using Neighbours = std::vector<types::Position>;

    /// Convenience function to make coordinates (x,y) into a Cell.
    /**
     * Takes coordinates (x,y) and makes them into a Cell.
     * @param x the x coordinate of the cell
     * @param y the y coordinate of the cell
     * @return a Cell representing (x,y)
     */
    inline Cell cell(int x, int y) { return std::make_pair(x, y); }

    /// Convenience function to make coordinates and a Direction into a maze Position.
    /**
     * Takes coordinates (x,y) and a Direction and creates a Position in the maze.
     * @param x the x coordinate of the position
     * @param y the y coordinate of the position
     * @param d the direction faced in the (x,y) cell
     * @return a Position representing Cell (x,y) and the Direction
     */
    inline Position pos(int x, int y, Direction d) { return std::make_pair(cell(x, y), d); }

    /// Convenience function to make a Cell and a Direction into a maze Position.
    /**
     * Takes a Cell and a Direction and creates a maze Position.
     * @param c the (x,y) cell in the maze
     * @param d the direction faced in the (x,y) cell
     * @return a Position representing the Cell and the Direction
     */
    inline Position pos(const Cell &c, Direction d) { return std::make_pair(c, d); }
}
