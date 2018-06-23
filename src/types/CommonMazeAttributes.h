/**
 * CommonMazeAttributes.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Attributes common to both regular and thick mazes.
 */

#pragma once

// We need to use Boost's optional instead of STL's optional since it doesn't work with serialization.
#include <boost/optional.hpp>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

#include "Direction.h"

namespace spelunker::types {
    class Dimensions2D;

    /// An (x,y) cell in a maze.
    using Cell = std::pair<int, int>;

    /// A collection of cells.
    using CellCollection = std::vector<Cell>;

    /// An indicator as to whether or not we've processed a Cell for a row.
    using CellRowIndicator = std::vector<bool>;

    /// An indicator as to whether or not we've processed a Cell.
    using CellIndicator = std::vector<CellRowIndicator>;

    /// Determine what direction two adjacent cells are apart.
    Direction cellDirection(const Cell &c1, const Cell &c2);

    /// A possible cell: it may be defined, or not.
    using PossibleCell = boost::optional<Cell>;

    /// A position in a maze, i.e. a Cell and a Direction.
    using Position = std::pair<Cell, Direction>;

    /// The neighbours of a cell. Walls should face towards the original cell.
    using Neighbours = std::vector<types::Position>;

    /// Initialize a cell indicator of the specified size.
    /**
     * Initialize a cell indicator, which is a matrix that indicates whether a cell has been processed
     * or visited.
     * @param width the width of the grid
     * @oaram height the height of the grid
     * @param def the starting state of each cell
     * @return the initialized cell indicator matrix
     */
    CellIndicator initializeCellIndicator(int width, int height, bool def = false);

    /// Initialize a cell indicator of the specified size.
    /**
     * Initialize a cell indicator, which is a matrix that indicates whether a cell has been processed
     * or visited.
     * @param d the dimension of the grid
     * @param def the starting state of each cell
     * @return the initialized cell indicator matrix
     */
    CellIndicator initializeCellIndicator(const Dimensions2D &d, bool def = false);

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

    /// An enumeration to specify the types of special cells in a maze.
    enum class SpecialCellType {
        START = 0,
        GOAL,
    };

    /// Represent a @see{SpecialCellType} by a descriptive string.
    std::string specialCellTypeName(const SpecialCellType c);
}
