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

        class IllegalPositionType : public Exception {
        public:
            IllegalPositionType(const Position &p, const MazeType t) : Exception(msg(p, t)) {}

        private:
            static std::string msg(const Position &p, const MazeType t) {
                return "Start position "
                       + vorpal::typeclasses::Show<Position>::show(p)
                       + " cannot occur on maze type "
                       + vorpal::typeclasses::Show<MazeType>::show(t)
                       + ".";

            }
        };

        class OutOfBoundsPosition : public Exception {
        public:
            OutOfBoundsPosition(const Position &p) : Exception(msg(p)) {}

        private:
            static std::string msg(const Position &p) {
                return "Position " + vorpal::typeclasses::Show<Position>::show(p) + " is out of bounds.";
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
