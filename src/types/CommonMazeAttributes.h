/**
 * CommonMazeAttributes.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Attributes common to both regular and thick mazes.
 */

#pragma once

#include <optional>
#include <string>
#include <tuple>
#include <vector>

namespace spelunker::types {
    /// An (x,y) cell in a maze.
    using Cell = std::pair<int, int>;

    /// A collection of cells.
    using CellCollection = std::vector<Cell>;

    /// An indicator as to whether or not we've processed a Cell for a row.
    using CellRowIndicator = std::vector<bool>;

    /// An indicator as to whether or not we've processed a Cell.
    using CellIndicator = std::vector<CellRowIndicator>;

    /// The four directions for each square in a maze.
    enum Direction {
        NORTH = 0,
        EAST,
        SOUTH,
        WEST,
    };

    /// Flip a direction.
    inline Direction flip(const Direction d) {
        switch (d) {
            case NORTH: return SOUTH;
            case EAST:  return WEST;
            case SOUTH: return NORTH;
            case WEST:  return EAST;
        }
    }

    /// A possible cell: it may be defined, or not.
    using PossibleCell = std::optional<Cell>;

    /// A list of all Directions for iteration.
    inline const std::vector<Direction> directions() {
        return std::vector<Direction> { NORTH, EAST, SOUTH, WEST };
    }

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

    /// Maze symmetries. Note that the diagonal reflections can only be performed for square mazes.
    /**
     * Maze symmetries. Since we work with (0,0) in the upper left corner, which is not standard in the euclidean
     * plane, we identify the reflections by their direction.
     */
    enum Symmetry {
        ROTATION_BY_90 = 0,
        ROTATION_BY_180,
        ROTATION_BY_270,
        REFLECTION_IN_Y,
        REFLECTION_IN_X,
        REFLECTION_IN_NWSE,
        REFLECTION_IN_NESW,
    };

    /// Get the name of a group symmetry.
    inline std::string symmetryName(const Symmetry s) {
        switch (s) {
            case ROTATION_BY_90:     return "rotation by 90\u00B0";
            case ROTATION_BY_180:    return "rotation by 180\u00B0";
            case ROTATION_BY_270:    return "rotation by 270\u00B0";
            case REFLECTION_IN_X:    return "reflection in x axis";
            case REFLECTION_IN_Y:    return "reflection in y axis";
            case REFLECTION_IN_NWSE: return "reflection in NW-SE diagonal";
            case REFLECTION_IN_NESW: return "reflection in NE-SW diagonal";
        }
    }

    /// Get the effect of a symmetry on a direction.
    inline types::Direction applySymmetryToDirection(const Symmetry s, const Direction d) {
        switch (s) {
            case ROTATION_BY_90:
                switch (d) {
                    case NORTH: return EAST;
                    case EAST:  return SOUTH;
                    case SOUTH: return WEST;
                    case WEST:  return NORTH;
                }
            case ROTATION_BY_180:
                return flip(d);
            case ROTATION_BY_270:
                switch (d) {
                    case NORTH: return WEST;
                    case EAST:  return NORTH;
                    case SOUTH: return EAST;
                    case WEST:  return SOUTH;
                }
            case REFLECTION_IN_X:
                switch (d) {
                    case NORTH: return SOUTH;
                    case SOUTH: return NORTH;
                    default:    return d;
                }
            case REFLECTION_IN_Y:
                switch (d) {
                    case EAST: return WEST;
                    case WEST: return EAST;
                    default:   return d;
                }
            case REFLECTION_IN_NWSE:
                switch (d) {
                    case NORTH: return WEST;
                    case EAST:  return SOUTH;
                    case SOUTH: return EAST;
                    case WEST:  return NORTH;
                }
            case REFLECTION_IN_NESW:
                switch (d) {
                    case NORTH: return EAST;
                    case EAST:  return NORTH;
                    case SOUTH: return WEST;
                    case WEST:  return SOUTH;
                }
        }
    }
}
