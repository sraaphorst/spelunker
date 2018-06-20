/**
 * Symmetry.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Symmetries that can be applied to mazes.
 */

#pragma once

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/version.hpp>

#include <string>

#include <typeclasses/Show.h>

#include "Direction.h"

namespace spelunker::types {
    class Dimensions2D;

    /// Maze symmetries. Note that the diagonal reflections can only be performed for square mazes.
    /**
     * Maze symmetries. Since we work with (0,0) in the upper left corner, which is not standard in the euclidean
     * plane, we identify the reflections by their direction.
     *
     * The group of symmetries of the square is as follows:
     *         id  R90 R180 R270   RX   RY  RNE  RNW
     *    id   id  R90 R180 R270   RX   RY  RNE  RNW
     *   R90  R90 R180 R270   id  RNE  RNW   RY   RX
     *  R180 R180 R270   id  R90   RY   RX  RNW  RNE
     *  R270 R270   id  R90 R180  RNW  RNE   RX   RY
     *    RX   RX  RNW   RY  RNE   id R180 R279  R90
     *    RY   RY  RNE   RX  RNW R180   id  R90 R270
     *   RNE  RNE   RX  RNW   RY  R90 R270   id R180
     *   RNW  RNW   RY  RNE   RX R270  R90 R180   id
     */
    enum class Symmetry {
        IDENTITY = 0,
        ROTATION_BY_90,
        ROTATION_BY_180,
        ROTATION_BY_270,
        REFLECTION_IN_Y,
        REFLECTION_IN_X,
        REFLECTION_IN_NESW,
        REFLECTION_IN_NWSE,
    };

    /// A list of all Symmetries for iteration.
    const std::vector<Symmetry> symmetries() noexcept;

    /// Function to serialize a Symmetry.
    template <typename Archive>
    void serialize(Archive &ar, Symmetry &d, const unsigned int version) {
        ar & d;
    }

    /// Compose two symmetries.
    /**
     * The symmetries of the square form a non-abelian group. Here is the Cayley table:
     *         id  R90 R180 R270   RX   RY  RNE  RNW
     *    id   id  R90 R180 R270   RX   RY  RNE  RNW
     *   R90  R90 R180 R270   id  RNE  RNW   RY   RX
     *  R180 R180 R270   id  R90   RY   RX  RNW  RNE
     *  R270 R270   id  R90 R180  RNW  RNE   RX   RY
     *    RX   RX  RNW   RY  RNE   id R180 R279  R90
     *    RY   RY  RNE   RX  RNW R180   id  R90 R270
     *   RNE  RNE   RX  RNW   RY  R90 R270   id R180
     *   RNW  RNW   RY  RNE   RX R270  R90 R180   id
     * This function, given two symmetries, returns their composition.
     * @param the first synmetry to apply
     * @param the second symmetry to apply
     * @return the composition of applying s1 followed by s2
     */
    Symmetry composeSymmetries(Symmetry s1, Symmetry s2) noexcept;

    /// Get a human-readable representation of a symmetry.
    std::string symmetryName(Symmetry s) noexcept;

    /// Determine the effects of a direction under the action of a symmetry.
    /**
     * Given a @see{Direction}, determine the effects that the given symmetry has on the
     * direction. (For example, a rotation by 90 degrees clockwise would take north and move it
     * to become east.)
     * @param s the symmetry
     * @param d the direction
     * @return the new direction under the effect of the symmetry
     */
    Direction applySymmetryToDirection(Symmetry s, Direction d) noexcept;

    /// Determine the effects of a symmetry on the dimensions.
    const Dimensions2D applySymmetryToDimensions(Symmetry s, const Dimensions2D &d) noexcept;
}

BOOST_CLASS_VERSION(spelunker::types::Symmetry, 1)

namespace spelunker::typeclasses {
    template<>
    struct Show<types::Symmetry > {
        static std::string show(const types::Symmetry s) {
            return types::symmetryName(s);
        }
        static constexpr bool is_instance = true;
        using type = types::Symmetry;
    };
}