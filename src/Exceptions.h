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
        /**
         * The root of exceptions throwable in this library.
         */
        class Exception : public std::logic_error {
        protected:
            explicit Exception(const std::string what) : std::logic_error(what) {}
        };

        class OutOfBoundsCell : public Exception {
        public:
            OutOfBoundsCell(const Cell &c) : Exception(msg(c)) {}

        private:
            static std::string msg(const Cell &c) {
                return "Cell " + vorpal::typeclasses::Show<Cell>::show(c) + " is out of bounds.";
            }
        };

        class IllegalDimensions : public Exception {
        public:
            IllegalDimensions(const int width, const int height) : Exception(msg(width, height)) {}

        private:
            static std::string msg(const int &width, const int &height) {
                return "Dimensions "
                       + vorpal::typeclasses::Show<std::pair<int,int>>::show(std::make_pair(width, height))
                       + "are not legal.";
            }
        };
    }
}

#endif //SPELUNKER_EXCEPTIONS_H
