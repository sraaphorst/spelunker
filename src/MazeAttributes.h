/**
 * MazeAttributes.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Attributes specific to Mazes.
 */

#ifndef SPELUNKER_MAZEATTRIBUTES_H
#define SPELUNKER_MAZEATTRIBUTES_H

#include <map>
#include <vector>

#include "Show.h"
#include "CommonMazeAttributes.h"

namespace spelunker::maze {
    using namespace spelunker::types;

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

    /// Calculates the number of possible internal (non-boundary) walls in a maze of width w and height h.
    inline const int calculateNumWalls(const int w, const int h) {
        return (w - 1) * h + w * (h - 1);
    }
}

namespace spelunker::typeclasses {
    using namespace spelunker::types;

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