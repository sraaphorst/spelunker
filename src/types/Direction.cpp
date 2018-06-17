/**
 * Direction.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <vector>

#include "Direction.h"

namespace spelunker::types {
    Direction flip(const Direction d) {
        switch (d) {
            case Direction::NORTH:
                return Direction::SOUTH;
            case Direction::EAST:
                return Direction::WEST;
            case Direction::SOUTH:
                return Direction::NORTH;
            case Direction::WEST:
                return Direction::EAST;
        }
    }

    const std::vector<Direction> directions() {
        return std::vector<Direction>{Direction::NORTH,
                                      Direction::EAST,
                                      Direction::SOUTH,
                                      Direction::WEST};
    }

    char directionToChar(Direction d) {
        switch (d) {
            case Direction::NORTH:
                return 'N';
            case Direction::EAST:
                return 'E';
            case Direction::SOUTH:
                return 'S';
            case Direction::WEST:
                return 'W';
        }
    }

    /// Get a string (all in lowercase) of a direction.
    std::string directionName(Direction d) {
        switch (d) {
            case Direction::NORTH:
                return "north";
            case Direction::EAST:
                return "east";
            case Direction::SOUTH:
                return "south";
            case Direction::WEST:
                return "west";
        }
    }
}