/**
 * Dimensions2D.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <boost/serialization/version.hpp>

#include <string>
#include <tuple>

#include <typeclasses/Show.h>

#include "CommonMazeAttributes.h"

namespace spelunker::types {
    /**
     * An immutable set of two dimensions.
     */
    class Dimensions2D final {
    public:
        /**
         * Create a new set of dimensions.
         * @param w the width
         * @param h the heigh
         */
        Dimensions2D(int w, int h);
        ~Dimensions2D() = default;

        /// Determine if two dimensions objects are the same.
        bool operator==(const Dimensions2D &other) const noexcept;

        /// Determine if two dimensions objects are different.
        bool operator!=(const Dimensions2D &other) const noexcept;

        /// Add two dimensions together.
        Dimensions2D operator+(const Dimensions2D &other) const noexcept;

        /// Multiply by a scalar.
        Dimensions2D operator*(int scalar) const noexcept;

        // Divide by a scalar (integer division).
        Dimensions2D operator/(int scalar) const;

        /// Get the width part of the dimensions.
        inline int getWidth() const noexcept {
            return width;
        }

        /// Get the height part of the dimensions.
        inline int getHeight() const noexcept {
            return height;
        }

        /// Convert to a tuple so that we can use binding.
        inline std::pair<int, int> values() const noexcept {
            return std::make_pair(width, height);
        }

        /// Determines if these dimensions are square, i.e. the width and the height are the same.
        inline bool isSquare() const noexcept {
            return width == height;
        }

        /// Determine whether or not the given coordinates are in bounds.
        /**
         * Check if the cell is in bounds, i.e. in x in [0,w) and y in [0,h).
         * @param c the coordinates of the cell
         * @return true if the cell is in bounds, and false otherwise.
         */
        bool cellInBounds(const Cell &c) const noexcept;

        /// Determine whether or not the given coordinates are in bounds.
        /**
         * Check if the cell is in bounds, i.e. in x in [0,w) and y in [0,h).
         * @param x the x coordinate
         * @param y the y coordinate
         * @return true if the cell is in bounds, and false otherwise.
         */
        bool cellInBounds(int x, int y) const noexcept;

        /// Checks is a cell is in bounds, and if not, throw an OutOfBoundsException.
        /**
         * Checks if a cell is in bounds, and if not, throws an OutOfBoundsException.
         * @param the cell
         * @throws OutOfBoundsException
         */
        void checkCell(const Cell &c) const;

        /// Checks is a cell is in bounds, and if not, throw an OutOfBoundsException.
        /**
         * Checks if a cell is in bounds, and if not, throws an OutOfBoundsException.
         * @param x the x coordinate
         * @param y the y coordinate
         * @throws OutOfBoundsException
         */
        void checkCell(int x, int y) const;

        static Dimensions2D load(std::istream &s);
        void save(std::ostream &s) const;

    private:
        const int width;
        const int height;

        /**
         * Empty constructor for serialization.
         */
        Dimensions2D() : width{0}, height{0} {};

        friend class boost::serialization::access;

        template<typename Archive>
        void serialize(Archive &ar, const unsigned int version);
    };

    inline Dimensions2D operator*(int scalar, const Dimensions2D dim) noexcept {
        return dim.operator*(scalar);
    }
}

BOOST_CLASS_VERSION(spelunker::types::Dimensions2D, 1)

namespace spelunker::typeclasses {
    template<>
    struct Show<types::Dimensions2D> {
        static std::string show(const types::Dimensions2D &d) {
            return typeclasses::Show<std::pair<int, int>>::show(std::make_pair(d.getWidth(), d.getHeight()));
        }
        static constexpr bool is_instance = true;
        using type = types::Dimensions2D;
    };
}
