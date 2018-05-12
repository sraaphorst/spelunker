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
        /// The types of terrain comprising a maze.
        /**
         * The three types of terrain possible in a maze:
         * FLOOR is empty floor
         * WALL is a closed wall
         * OUTOFBOUNDS is a square that is marked as not able to use for any algorithms,
         *             and one that cannot be traversed. This can come in handy for making
         *             interestingly-shaped mazes.
         * EMPTY is an unused space, and can be used by the maze generation algorithms.
         */
        enum MazeType {
            FLOOR,
            WALL,
            UNUSABLE,
            EMPTY,
        };

        /// An (x,y) position in a maze.
        using Position = std::pair<int, int>;

        /// The layout of a maze, as a rectangle.
        using Layout = std::vector<std::vector<MazeType>>;

        /// A collection of positions.
        using PositionCollection = std::set<Position>;
    }
}

namespace vorpal {
    namespace typeclasses {
        using namespace vorpal::maze;

        template<>
        struct vorpal::typeclasses::Show<MazeType> {
            static std::string show(const MazeType &t) {
                switch (t) {
                    case FLOOR:    return "FLOOR";
                    case WALL:     return "WALL";
                    case UNUSABLE: return "UNUSABLE";
                    case EMPTY:    return "EMPTY";
                    default:       return "UNDEFINED";
                }
            }
            static constexpr bool is_instance = true;
            using type = MazeType;
        };
    }
}

#endif //SPELUNKER_MAZEATTRIBUTES_H