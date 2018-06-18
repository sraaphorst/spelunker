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
    std::string symmetryName(const Symmetry s) noexcept {
        switch (s) {
            case Symmetry::IDENTITY:
                return "Identity";
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
            case Symmetry::REFLECTION_IN_NESW:
                return "Reflection in NE-SW diagonal";
            case Symmetry::REFLECTION_IN_NWSE:
                return "Reflection in NW-SE diagonal";
        }
    }

    const std::vector<Symmetry> symmetries() noexcept {
        return std::vector<Symmetry> {
            Symmetry::IDENTITY,
            Symmetry::ROTATION_BY_90,
            Symmetry::ROTATION_BY_180,
            Symmetry::ROTATION_BY_270,
            Symmetry::REFLECTION_IN_X,
            Symmetry::REFLECTION_IN_Y,
            Symmetry::REFLECTION_IN_NESW,
            Symmetry::REFLECTION_IN_NWSE
        };
    }

    /**
     *         id  R90 R180 R270   RX   RY  RNE  RNW
     *    id   id  R90 R180 R270   RX   RY  RNE  RNW
     *   R90  R90 R180 R270   id  RNE  RNW   RY   RX
     *  R180 R180 R270   id  R90   RY   RX  RNW  RNE
     *  R270 R270   id  R90 R180  RNW  RNE   RX   RY
     *    RX   RX  RNW   RY  RNE   id R180 R270  R90
     *    RY   RY  RNE   RX  RNW R180   id  R90 R270
     *   RNE  RNE   RX  RNW   RY  R90 R270   id R180
     *   RNW  RNW   RY  RNE   RX R270  R90 R180   id
     */
    Symmetry composeSymmetries(Symmetry s1, Symmetry s2) noexcept {
        switch (s1) {
            case Symmetry::IDENTITY:
                return s2;

            case Symmetry::ROTATION_BY_90:
                switch (s2) {
                    case Symmetry::IDENTITY:
                        return Symmetry::ROTATION_BY_90;
                    case Symmetry::ROTATION_BY_90:
                        return Symmetry::ROTATION_BY_180;
                    case Symmetry::ROTATION_BY_180:
                        return Symmetry::ROTATION_BY_270;
                    case Symmetry::ROTATION_BY_270:
                        return Symmetry::IDENTITY;
                    case Symmetry::REFLECTION_IN_X:
                        return Symmetry::REFLECTION_IN_NESW;
                    case Symmetry::REFLECTION_IN_Y:
                        return Symmetry::REFLECTION_IN_NWSE;
                    case Symmetry::REFLECTION_IN_NESW:
                        return Symmetry::REFLECTION_IN_Y;
                    case Symmetry::REFLECTION_IN_NWSE:
                        return Symmetry::REFLECTION_IN_X;
                }

            case Symmetry::ROTATION_BY_180:
                switch (s2) {
                    case Symmetry::IDENTITY:
                        return Symmetry::ROTATION_BY_180;
                    case Symmetry::ROTATION_BY_90:
                        return Symmetry::ROTATION_BY_270;
                    case Symmetry::ROTATION_BY_180:
                        return Symmetry::IDENTITY;
                    case Symmetry::ROTATION_BY_270:
                        return Symmetry::ROTATION_BY_90;
                    case Symmetry::REFLECTION_IN_Y:
                        return Symmetry::REFLECTION_IN_NESW;
                    case Symmetry::REFLECTION_IN_X:
                        return Symmetry::REFLECTION_IN_NWSE;
                    case Symmetry::REFLECTION_IN_NESW:
                        return Symmetry::REFLECTION_IN_Y;
                    case Symmetry::REFLECTION_IN_NWSE:
                        return Symmetry::REFLECTION_IN_X;
                }

            case Symmetry::ROTATION_BY_270:
                switch (s2) {
                    case Symmetry::IDENTITY:
                        return Symmetry::ROTATION_BY_270;
                    case Symmetry::ROTATION_BY_90:
                        return Symmetry::IDENTITY;
                    case Symmetry::ROTATION_BY_180:
                        return Symmetry::ROTATION_BY_90;
                    case Symmetry::ROTATION_BY_270:
                        return Symmetry::ROTATION_BY_180;
                    case Symmetry::REFLECTION_IN_Y:
                        return Symmetry::REFLECTION_IN_NWSE;
                    case Symmetry::REFLECTION_IN_X:
                        return Symmetry::REFLECTION_IN_NESW;
                    case Symmetry::REFLECTION_IN_NESW:
                        return Symmetry::REFLECTION_IN_X;
                    case Symmetry::REFLECTION_IN_NWSE:
                        return Symmetry::REFLECTION_IN_Y;
                }

            case Symmetry::REFLECTION_IN_X:
                switch (s2) {
                    case Symmetry::IDENTITY:
                        return Symmetry::REFLECTION_IN_X;
                    case Symmetry::ROTATION_BY_90:
                        return Symmetry::REFLECTION_IN_NWSE;
                    case Symmetry::ROTATION_BY_180:
                        return Symmetry::REFLECTION_IN_Y;
                    case Symmetry::ROTATION_BY_270:
                        return Symmetry::REFLECTION_IN_NESW;
                    case Symmetry::REFLECTION_IN_X:
                        return Symmetry::IDENTITY;
                    case Symmetry::REFLECTION_IN_Y:
                        return Symmetry::ROTATION_BY_180;
                    case Symmetry::REFLECTION_IN_NESW:
                        return Symmetry::ROTATION_BY_270;
                    case Symmetry::REFLECTION_IN_NWSE:
                        return Symmetry::ROTATION_BY_90;
                }

            case Symmetry::REFLECTION_IN_Y:
                switch (s2) {
                    case Symmetry::IDENTITY:
                        return Symmetry::REFLECTION_IN_Y;
                    case Symmetry::ROTATION_BY_90:
                        return Symmetry::REFLECTION_IN_NESW;
                    case Symmetry::ROTATION_BY_180:
                        return Symmetry::REFLECTION_IN_X;
                    case Symmetry::ROTATION_BY_270:
                        return Symmetry::REFLECTION_IN_NWSE;
                    case Symmetry::REFLECTION_IN_X:
                        return Symmetry::ROTATION_BY_180;
                    case Symmetry::REFLECTION_IN_Y:
                        return Symmetry::IDENTITY;
                    case Symmetry::REFLECTION_IN_NESW:
                        return Symmetry::ROTATION_BY_90;
                    case Symmetry::REFLECTION_IN_NWSE:
                        return Symmetry::ROTATION_BY_270;
                }

            case Symmetry::REFLECTION_IN_NESW:
                switch (s2) {
                    case Symmetry::IDENTITY:
                        return Symmetry::REFLECTION_IN_NESW;
                    case Symmetry::ROTATION_BY_90:
                        return Symmetry::REFLECTION_IN_X;
                    case Symmetry::ROTATION_BY_180:
                        return Symmetry::REFLECTION_IN_NWSE;
                    case Symmetry::ROTATION_BY_270:
                        return Symmetry::REFLECTION_IN_Y;
                    case Symmetry::REFLECTION_IN_X:
                        return Symmetry::ROTATION_BY_90;
                    case Symmetry::REFLECTION_IN_Y:
                        return Symmetry::ROTATION_BY_270;
                    case Symmetry::REFLECTION_IN_NESW:
                        return Symmetry::IDENTITY;
                    case Symmetry::REFLECTION_IN_NWSE:
                        return Symmetry::ROTATION_BY_180;
                }

            case Symmetry::REFLECTION_IN_NWSE:
                switch (s2) {
                    case Symmetry::IDENTITY:
                        return Symmetry::REFLECTION_IN_NWSE;
                    case Symmetry::ROTATION_BY_90:
                        return Symmetry::REFLECTION_IN_Y;
                    case Symmetry::ROTATION_BY_180:
                        return Symmetry::REFLECTION_IN_NESW;
                    case Symmetry::ROTATION_BY_270:
                        return Symmetry::REFLECTION_IN_X;
                    case Symmetry::REFLECTION_IN_X:
                        return Symmetry::ROTATION_BY_270;
                    case Symmetry::REFLECTION_IN_Y:
                        return Symmetry::ROTATION_BY_90;
                    case Symmetry::REFLECTION_IN_NESW:
                        return Symmetry::ROTATION_BY_180;
                    case Symmetry::REFLECTION_IN_NWSE:
                        return Symmetry::IDENTITY;
                }
        }
    }

    Direction applySymmetryToDirection(const Symmetry s, const Direction d) noexcept {
        switch (s) {
            case Symmetry::IDENTITY:
                return d;

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
                    default:
                        return d;
                }
            case Symmetry::REFLECTION_IN_Y:
                switch (d) {
                    case Direction::EAST:
                        return Direction::WEST;
                    case Direction::WEST:
                        return Direction::EAST;
                    default:
                        return d;
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
        }
    }

    const Dimensions2D applySymmetryToDimensions(Symmetry s, const Dimensions2D &d) noexcept {
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