/**
 * Exceptions.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#ifndef SPELUNKER_EXCEPTIONS_H
#define SPELUNKER_EXCEPTIONS_H

#include <stdexcept>
#include <string>
#include <tuple>

#include "MazeAttributes.h"
#include "Show.h"

namespace vorpal {
    namespace maze {
        /// The root of all non-STL exceptions throwable by this library.
        class Exception : public std::logic_error {
        protected:
            explicit Exception(const std::string what) : std::logic_error(what) {}
        };

        /// Thrown if a cell out of bounds of the maze is accessed.
        class OutOfBoundsCell : public Exception {
        public:
            OutOfBoundsCell(const types::Cell &c) : Exception(msg(c)) {}

        private:
            static std::string msg(const types::Cell &c) {
                return "Cell " + vorpal::typeclasses::Show<types::Cell>::show(c) + " is out of bounds.";
            }
        };

        /// Thrown if the user tries to create a maze with illegal dimensions.
        class IllegalDimensions : public Exception {
        public:
            IllegalDimensions(const int width, const int height) : Exception(msg(width, height)) {}

        private:
            static std::string msg(const int width, const int height) {
                return "Dimensions "
                       + vorpal::typeclasses::Show<std::pair<int,int>>::show(std::make_pair(width, height))
                       + "are not legal.";
            }
        };

        /// Thrown if the user tries to perform an illegal transformation on the maze.
        /**
         * Thrown if the user tries to perform an illegal transformation on the maze.
         * This happens when attempting to reflect in a diagonal axis and the width and height differ.
         */
        class IllegalGroupOperation : public Exception {
        public:
            IllegalGroupOperation(const int width, const int height, const types::Symmetry s)
                    : Exception(msg(width, height, s)) {}

        private:
            static std::string msg(const int width, const int height, const types::Symmetry s) {
                return "width " + std::to_string(width) + " != " + std::to_string(height)
                       + ", so cannot perform symmetry " + types::symmetryName(s);
            }
        };
    }
}

#endif //SPELUNKER_EXCEPTIONS_H
