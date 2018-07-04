/**
 * Transformation.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <string>

#include "Dimensions2D.h"
#include "Direction.h"
#include "Transformation.h"

namespace spelunker::types {
    std::string transformationName(Transformation t) noexcept {
        switch (t) {
            case Transformation::IDENTITY:
                return "Identity";
            case Transformation::ROTATION_BY_90:
                return "Rotation by 90\u00B0";
            case Transformation::ROTATION_BY_180:
                return "Rotation by 180\u00B0";
            case Transformation::ROTATION_BY_270:
                return "Rotation by 270\u00B0";
            case Transformation::REFLECTION_IN_X:
                return "Reflection in x axis";
            case Transformation::REFLECTION_IN_Y:
                return "Reflection in y axis";
            case Transformation::REFLECTION_IN_NESW:
                return "Reflection in NE-SW diagonal";
            case Transformation::REFLECTION_IN_NWSE:
                return "Reflection in NW-SE diagonal";
        }
    }

    const std::vector<Transformation> transformations() noexcept {
        return std::vector<Transformation> {
            Transformation::IDENTITY,
            Transformation::ROTATION_BY_90,
            Transformation::ROTATION_BY_180,
            Transformation::ROTATION_BY_270,
            Transformation::REFLECTION_IN_X,
            Transformation::REFLECTION_IN_Y,
            Transformation::REFLECTION_IN_NESW,
            Transformation::REFLECTION_IN_NWSE
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
    Transformation composeTransformations(Transformation t1, Transformation t2) noexcept {
        switch (t1) {
            case Transformation::IDENTITY:
                return t2;

            case Transformation::ROTATION_BY_90:
                switch (t2) {
                    case Transformation::IDENTITY:
                        return Transformation::ROTATION_BY_90;
                    case Transformation::ROTATION_BY_90:
                        return Transformation::ROTATION_BY_180;
                    case Transformation::ROTATION_BY_180:
                        return Transformation::ROTATION_BY_270;
                    case Transformation::ROTATION_BY_270:
                        return Transformation::IDENTITY;
                    case Transformation::REFLECTION_IN_X:
                        return Transformation::REFLECTION_IN_NESW;
                    case Transformation::REFLECTION_IN_Y:
                        return Transformation::REFLECTION_IN_NWSE;
                    case Transformation::REFLECTION_IN_NESW:
                        return Transformation::REFLECTION_IN_Y;
                    case Transformation::REFLECTION_IN_NWSE:
                        return Transformation::REFLECTION_IN_X;
                }

            case Transformation::ROTATION_BY_180:
                switch (t2) {
                    case Transformation::IDENTITY:
                        return Transformation::ROTATION_BY_180;
                    case Transformation::ROTATION_BY_90:
                        return Transformation::ROTATION_BY_270;
                    case Transformation::ROTATION_BY_180:
                        return Transformation::IDENTITY;
                    case Transformation::ROTATION_BY_270:
                        return Transformation::ROTATION_BY_90;
                    case Transformation::REFLECTION_IN_Y:
                        return Transformation::REFLECTION_IN_X;
                    case Transformation::REFLECTION_IN_X:
                        return Transformation::REFLECTION_IN_Y;
                    case Transformation::REFLECTION_IN_NESW:
                        return Transformation::REFLECTION_IN_NWSE;
                    case Transformation::REFLECTION_IN_NWSE:
                        return Transformation::REFLECTION_IN_NESW;
                }

            case Transformation::ROTATION_BY_270:
                switch (t2) {
                    case Transformation::IDENTITY:
                        return Transformation::ROTATION_BY_270;
                    case Transformation::ROTATION_BY_90:
                        return Transformation::IDENTITY;
                    case Transformation::ROTATION_BY_180:
                        return Transformation::ROTATION_BY_90;
                    case Transformation::ROTATION_BY_270:
                        return Transformation::ROTATION_BY_180;
                    case Transformation::REFLECTION_IN_X:
                        return Transformation::REFLECTION_IN_NWSE;
                    case Transformation::REFLECTION_IN_Y:
                        return Transformation::REFLECTION_IN_NESW;
                    case Transformation::REFLECTION_IN_NESW:
                        return Transformation::REFLECTION_IN_X;
                    case Transformation::REFLECTION_IN_NWSE:
                        return Transformation::REFLECTION_IN_Y;
                }

            case Transformation::REFLECTION_IN_X:
                switch (t2) {
                    case Transformation::IDENTITY:
                        return Transformation::REFLECTION_IN_X;
                    case Transformation::ROTATION_BY_90:
                        return Transformation::REFLECTION_IN_NWSE;
                    case Transformation::ROTATION_BY_180:
                        return Transformation::REFLECTION_IN_Y;
                    case Transformation::ROTATION_BY_270:
                        return Transformation::REFLECTION_IN_NESW;
                    case Transformation::REFLECTION_IN_X:
                        return Transformation::IDENTITY;
                    case Transformation::REFLECTION_IN_Y:
                        return Transformation::ROTATION_BY_180;
                    case Transformation::REFLECTION_IN_NESW:
                        return Transformation::ROTATION_BY_270;
                    case Transformation::REFLECTION_IN_NWSE:
                        return Transformation::ROTATION_BY_90;
                }

            case Transformation::REFLECTION_IN_Y:
                switch (t2) {
                    case Transformation::IDENTITY:
                        return Transformation::REFLECTION_IN_Y;
                    case Transformation::ROTATION_BY_90:
                        return Transformation::REFLECTION_IN_NESW;
                    case Transformation::ROTATION_BY_180:
                        return Transformation::REFLECTION_IN_X;
                    case Transformation::ROTATION_BY_270:
                        return Transformation::REFLECTION_IN_NWSE;
                    case Transformation::REFLECTION_IN_X:
                        return Transformation::ROTATION_BY_180;
                    case Transformation::REFLECTION_IN_Y:
                        return Transformation::IDENTITY;
                    case Transformation::REFLECTION_IN_NESW:
                        return Transformation::ROTATION_BY_90;
                    case Transformation::REFLECTION_IN_NWSE:
                        return Transformation::ROTATION_BY_270;
                }

            case Transformation::REFLECTION_IN_NESW:
                switch (t2) {
                    case Transformation::IDENTITY:
                        return Transformation::REFLECTION_IN_NESW;
                    case Transformation::ROTATION_BY_90:
                        return Transformation::REFLECTION_IN_X;
                    case Transformation::ROTATION_BY_180:
                        return Transformation::REFLECTION_IN_NWSE;
                    case Transformation::ROTATION_BY_270:
                        return Transformation::REFLECTION_IN_Y;
                    case Transformation::REFLECTION_IN_X:
                        return Transformation::ROTATION_BY_90;
                    case Transformation::REFLECTION_IN_Y:
                        return Transformation::ROTATION_BY_270;
                    case Transformation::REFLECTION_IN_NESW:
                        return Transformation::IDENTITY;
                    case Transformation::REFLECTION_IN_NWSE:
                        return Transformation::ROTATION_BY_180;
                }

            case Transformation::REFLECTION_IN_NWSE:
                switch (t2) {
                    case Transformation::IDENTITY:
                        return Transformation::REFLECTION_IN_NWSE;
                    case Transformation::ROTATION_BY_90:
                        return Transformation::REFLECTION_IN_Y;
                    case Transformation::ROTATION_BY_180:
                        return Transformation::REFLECTION_IN_NESW;
                    case Transformation::ROTATION_BY_270:
                        return Transformation::REFLECTION_IN_X;
                    case Transformation::REFLECTION_IN_X:
                        return Transformation::ROTATION_BY_270;
                    case Transformation::REFLECTION_IN_Y:
                        return Transformation::ROTATION_BY_90;
                    case Transformation::REFLECTION_IN_NESW:
                        return Transformation::ROTATION_BY_180;
                    case Transformation::REFLECTION_IN_NWSE:
                        return Transformation::IDENTITY;
                }
        }
    }

    Direction applyTransformationToDirection(Transformation t, Direction d) noexcept {
        switch (t) {
            case Transformation::IDENTITY:
                return d;

            case Transformation::ROTATION_BY_90:
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
            case Transformation::ROTATION_BY_180:
                return flip(d);
            case Transformation::ROTATION_BY_270:
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
            case Transformation::REFLECTION_IN_X:
                switch (d) {
                    case Direction::NORTH:
                        return Direction::SOUTH;
                    case Direction::SOUTH:
                        return Direction::NORTH;
                    default:
                        return d;
                }
            case Transformation::REFLECTION_IN_Y:
                switch (d) {
                    case Direction::EAST:
                        return Direction::WEST;
                    case Direction::WEST:
                        return Direction::EAST;
                    default:
                        return d;
                }
            case Transformation::REFLECTION_IN_NESW:
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
            case Transformation::REFLECTION_IN_NWSE:
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

    const Dimensions2D applyTransformationToDimensions(Transformation t, const Dimensions2D &d) noexcept {
        int nWidth, nHeight;
        if (t == Transformation::IDENTITY
            || t == Transformation::ROTATION_BY_180
            || t == Transformation::REFLECTION_IN_X
            || t == Transformation::REFLECTION_IN_Y) {
            nWidth = d.getWidth();
            nHeight = d.getHeight();
        } else {
            nWidth = d.getHeight();
            nHeight = d.getWidth();
        }
        return Dimensions2D{nWidth, nHeight};
    }
}