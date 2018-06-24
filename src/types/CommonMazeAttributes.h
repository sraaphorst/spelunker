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
#include <set>
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

    /// A way of indicating cells at certain distances from a given cell.
    /**
     * Given a cell c (the only cell in the set at position 0 of the vector),
     * we receive a vector of cells where the cells in the set at position i
     * of the vector are the cells that have a shortest path of length i from
     * the initial cell c.
     */
    using CellDistances = std::vector<std::set<Cell>>;

    /// The results of a BFS.
    /**
     * Performing a BFS starting at a fixed cell, the BFS will return the
     * following data structure, comprising:
     * 1. The initial cell (given).
     * 2. A CellCollection of all cells visited.
     * 3. A CellDistances structure recording the distances of the cells in
     *    (2) above.
     */
    struct BFSResults {
        const Cell start;
        const CellCollection connectedCells;
        const CellDistances distances
    };

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

    /// Apply a Direction to a Cell to get the new coordinates of the new Cell.
    /**
     * Given a Cell and a Direction, apply the Direction to the Cell to find the
     * new Cell that would be encountered by moving in that Direction.
     * Note that there is no guarantee that that the returned cell is in
     * bounds, as this just applies the direction to the (x,y) coordinates.
     * @params c the cell in consideration
     * @params d the directon in which to move
     * @return coordinates of the cell (perhaps illegal) in direction d from c
     */
    const Cell &applyDirectionToCell(const Cell &c, Direction d) noexcept;

    /// An enumeration to specify the types of special cells in a maze.
    enum class SpecialCellType {
        START = 0,
        GOAL,
    };

    /// Represent a @see{SpecialCellType} by a descriptive string.
    std::string specialCellTypeName(const SpecialCellType c);
}
