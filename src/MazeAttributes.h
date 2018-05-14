/**
 * MazeAttributes.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#ifndef SPELUNKER_MAZEATTRIBUTES_H
#define SPELUNKER_MAZEATTRIBUTES_H

#include <map>
#include <set>
#include <tuple>
#include <vector>
#include "Show.h"

namespace vorpal {
    namespace maze {
        /// An (x,y) cell in a maze.
        using Cell = std::pair<int, int>;

        /// A collection of cells.
        using CellCollection = std::vector<Cell>;

        /// The four directions for each square in a maze.
        enum Direction {
            NORTH = 0,
            EAST,
            SOUTH,
            WEST,
        };

        /// A list of all Directions for iteration.
        const std::vector<Direction> Directions{NORTH, EAST, SOUTH, WEST};

        /// A position in a maze, i.e. a Cell and a Direction.
        using Position = std::pair<Cell, Direction>;

        /// The ID for a wall. Walls are ranked to ints.
        using WallID = int;

        /// The wall incidence of a maze.
        /**
         * The incidence of walls in a maze, which is a map from the rank of the wall to a bool indicating
         * its existence. This, with Layout, determines the maze. We represent this by vector since we assume
         * a contiguous range of wall incidences.
         */
        using WallIncidence = std::vector<bool>;

        /// Used to reverse wall ranking, i.e. a map that takes a wall rank and gives the two cells it separates.
        using UnrankWallMap = std::map< WallID, const std::pair< Position, Position > >;
    }
}

namespace vorpal {
    namespace typeclasses {
        using namespace vorpal::maze;

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
}

#endif //SPELUNKER_MAZEATTRIBUTES_H