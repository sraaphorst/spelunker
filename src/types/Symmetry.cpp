/**
 * Symmetry.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <string>

#include "Dimensions2D.h"
#include "Direction.h"
#include "Symmetry.h"

namespace spelunker::types {
    /// Get the name of a group symmetry.
    std::string symmetryName(const Symmetry s) {
        switch (s) {
            case Symmetry::ROTATION_BY_90:
                return "Rotation by 90\u00B0";
            case Symmetry::ROTATION_BY_180:
                return "Rotation by 180\u00B0";
            case Symmetry::ROTATION_BY_270:
                return "Rotation by 270\u00B0";
            case Symmetry::REFLECTION_IN_X:
                return "Reflection in x axis";
            case Symmetry::REFLECTION_IN_Y:
                return "Reflection in y axis";
            case Symmetry::REFLECTION_IN_NWSE:
                return "Reflection in NW-SE diagonal";
            case Symmetry::REFLECTION_IN_NESW:
                return "Reflection in NE-SW diagonal";
        }
    }


    /// Get the effect of a symmetry on a direction.
    Direction applySymmetryToDirection(const Symmetry s, const Direction d) {
        switch (s) {
            case Symmetry::ROTATION_BY_90:
                switch (d) {
                    case Direction::NORTH:
                        return Direction::EAST;
                    case Direction::EAST:
                        return Direction::SOUTH;
                    case Direction::SOUTH:
                        return Direction::WEST;
                    case Direction::WEST:
                        return Direction::NORTH;
                }
            case Symmetry::ROTATION_BY_180:
                return flip(d);
            case Symmetry::ROTATION_BY_270:
                switch (d) {
                    case Direction::NORTH:
                        return Direction::WEST;
                    case Direction::EAST:
                        return Direction::NORTH;
                    case Direction::SOUTH:
                        return Direction::EAST;
                    case Direction::WEST:
                        return Direction::SOUTH;
                }
            case Symmetry::REFLECTION_IN_X:
                switch (d) {
                    case Direction::NORTH:
                        return Direction::SOUTH;
                    case Direction::SOUTH:
                        return Direction::NORTH;
                    default:    return d;
                }
            case Symmetry::REFLECTION_IN_Y:
                switch (d) {
                    case Direction::EAST:
                        return Direction::WEST;
                    case Direction::WEST:
                        return Direction::EAST;
                    default:   return d;
                }
            case Symmetry::REFLECTION_IN_NWSE:
                switch (d) {
                    case Direction::NORTH:
                        return Direction::WEST;
                    case Direction::EAST:
                        return Direction::SOUTH;
                    case Direction::SOUTH:
                        return Direction::EAST;
                    case Direction::WEST:
                        return Direction::NORTH;
                }
            case Symmetry::REFLECTION_IN_NESW:
                switch (d) {
                    case Direction::NORTH:
                        return Direction::EAST;
                    case Direction::EAST:
                        return Direction::NORTH;
                    case Direction::SOUTH:
                        return Direction::WEST;
                    case Direction::WEST:
                        return Direction::SOUTH;
                }
        }
    }

    const Dimensions2D applySymmetryToDimensions(Symmetry s, const Dimensions2D &d) {
        int nWidth, nHeight;
        if (s == Symmetry::ROTATION_BY_180
            || s == Symmetry::REFLECTION_IN_X
            || s == Symmetry::REFLECTION_IN_Y) {
            nWidth = d.getWidth();
            nHeight = d.getHeight();
        } else {
            nWidth = d.getHeight();
            nHeight = d.getWidth();
        }
        return Dimensions2D{nWidth, nHeight};
    }
}