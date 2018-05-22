/**
 * MazeAttributes.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#ifndef SPELUNKER_MAZEATTRIBUTES_H
#define SPELUNKER_MAZEATTRIBUTES_H

#include <map>
#include <optional>
#include <set>
#include <tuple>
#include <vector>

#include "Show.h"

namespace vorpal::maze::types {
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

    /// The type of a starting cell: it may be defined, or not.
    using PossibleStartCell = std::optional<Cell>;

    /// A list of all Directions for iteration.
    inline const std::vector<Direction> directions() {
        return std::vector<Direction> { NORTH, EAST, SOUTH, WEST };
    }

    /// A position in a maze, i.e. a Cell and a Direction.
    using Position = std::pair<Cell, Direction>;

    /// The neighbours of a cell. Walls should face towards the original cell.
    using Neighbours = std::vector<types::Position>;

    /// The ID for a wall. Walls are ranked to ints.
    using WallID = int;

    /// The wall incidence of a maze.
    /**
     * The incidence of walls in a maze, which is a map from the rank of the wall to a bool indicating
     * its existence. This, with Layout, determines the maze. We represent this by vector since we assume
     * a contiguous range of wall incidences.
     */
    using WallIncidence = std::vector<bool>;

    /// A collection of walls for processing.
    using WallCollection = std::vector<WallID>;

    /// Used to reverse wall ranking, i.e. a map that takes a wall rank and gives the two cells it separates.
    using UnrankWallMap = std::map<WallID, std::pair<Position, Position> >;

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

    /// Calculates the number of possible internal (non-boundary) walls in a maze of width w and height h.
    inline const int calculateNumWalls(const int w, const int h) {
        return (w - 1) * h + w * (h - 1);
    }
}

namespace vorpal::typeclasses {
    using namespace vorpal::maze::types;

    template<>
    struct Show<Direction> {
        static std::string show(const Direction &t) {
            switch (t) {
                case NORTH:
                    return "NORTH";
                case EAST:
                    return "EAST";
                case SOUTH:
                    return "SOUTH";
                case WEST:
                    return "WEST";
                default:
                    return "UNDEFINED";
            }
        }

        static constexpr bool is_instance = true;
        using type = Direction;
    };
}

#endif //SPELUNKER_MAZEATTRIBUTES_H