/**
 * Exceptions.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <stdexcept>
#include <string>
#include <tuple>

#include <maze/MazeAttributes.h>
#include <typeclasses/Show.h>

//#include "Dimensions2D.h"
#include "Symmetry.h"

namespace spelunker::types {

    /// The root of all non-STL exceptions throwable by this library.
    class Exception : public std::logic_error {
    protected:
        explicit Exception(const std::string &what) : std::logic_error(what) {}
    };

    /// Thrown if coordinates out of bounds of the maze are attempted to be accessed.
    class OutOfBoundsCoordinates : public Exception {
    public:
        OutOfBoundsCoordinates(const int x, const int y) : Exception(msg(x, y)) {}

    private:
        static std::string msg(const int x, const int y) {
            return "Cell "
                   + typeclasses::Show<types::Cell>::show(std::make_pair(x, y))
                   + " is out of bounds.";
        }
    };

    /// Thrown if the user tries to create a maze with illegal dimensions.
    class IllegalDimensions : public Exception {
    public:
        IllegalDimensions(const int width, const int height) : Exception(msg(std::make_pair(width, height))) {}

    private:
        static std::string msg(const std::pair<int, int> &d) {
            return "Dimensions2D "
                   + typeclasses::Show<std::pair<int,int>>::show(d)
                   + " are not legal.";
        }
    };

    /// Thrown if the user tries to perform an illegal transformation on the maze.
    /**
     * Thrown if the user tries to perform an illegal transformation on the maze.
     * This happens when attempting to reflect in a diagonal axis and the width and height differ.
     */
    class IllegalGroupOperation : public Exception {
    public:
        IllegalGroupOperation(const Dimensions2D &d, const types::Symmetry s) : Exception(msg(d, s)) {}
        IllegalGroupOperation(const int width, const int height, const types::Symmetry s)
                : IllegalGroupOperation(Dimensions2D{width, height}, s) {}

    private:
        static std::string msg(const Dimensions2D &d, const types::Symmetry s) {
            return "width " + typeclasses::Show<int>::show(d.getWidth())
                   + " != height " + typeclasses::Show<int>::show(d.getHeight())
                   + ", so cannot perform symmetry: " + typeclasses::Show<types::Symmetry>::show(s);
        }
    };

    /// Thrown if the user tries to specify a start or goal cell that is not legally placed.
    /**
     * Thrown if the user tries to specify a start or goal cell that is not legally placed.
     * Note that this is reserved strictly for cells that is <b>in bounds.</b>
     * Cells that are <b>out of bounds</b> are reported by @see{OutOfBoundsCoordinates}.
     */
     class IllegalCellPosition : public Exception {
     public:
         IllegalCellPosition(const types::Cell &c, const types::SpecialCellType ct) : Exception{msg(c, ct)} {}
         IllegalCellPosition(const int x, const int y, const types::SpecialCellType ct)
            : IllegalCellPosition{types::cell(x, y), ct} {}

     private:
         static std::string msg(const types::Cell &c, const types::SpecialCellType ct) {
             return "The cell " + typeclasses::Show<types::Cell>::show(c) + " is not a legal " +
                     types::specialCellTypeName(ct) + " cell.";
         }
     };
}
