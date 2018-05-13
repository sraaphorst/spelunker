/**
 * MazeAttributes.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#ifndef SPELUNKER_MAZEATTRIBUTES_H
#define SPELUNKER_MAZEATTRIBUTES_H

#include <ostream>
#include <set>
#include <tuple>
#include <vector>
#include "Show.h"

namespace vorpal {
    namespace maze {
        /// The four directions for each square in a maze.
        enum Direction {
            NORTH = 0,
            EAST,
            SOUTH,
            WEST,
        };

        /// An (x,y) position in a maze.
        using Position = std::pair<int, int>;

        /// A cell in the maze.
        using Cell = std::vector< bool >;

        /// A row in the maze.
        using Row = std::vector< Cell >;

        /// The layout of a maze.
        /**
         * The layout of a maze, as a rectangle, indexed by (x,y,d), where (x,y) is the Position and d is the
         * Direction. The value is true if there is a wall, and false otherwise.
         */
        using Layout = std::vector< Row >;

        /// A collection of positions.
        using PositionCollection = std::set< Position >;
    }
}

namespace vorpal {
    namespace typeclasses {
        using namespace vorpal::maze;

        template<>
        struct vorpal::typeclasses::Show< Direction > {
            static std::string show(const Direction &t) {
                switch (t) {
                    case NORTH: return "NORTH";
                    case EAST:  return "EAST";
                    case SOUTH: return "SOUTH";
                    case WEST:  return "WEST";
                    default:    return "UNDEFINED";
                }
            }
            static constexpr bool is_instance = true;
            using type = Direction;
        };
    }
}

#endif //SPELUNKER_MAZEATTRIBUTES_H