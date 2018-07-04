/**
 * Transformation.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Transformations that can be applied to mazes.
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

    /// Standard maze transformations. Note that the diagonal reflections can only be performed for square mazes.
    /**
     * Maze transformations. Since we work with (0,0) in the upper left corner, which is not standard in the euclidean
     * plane, we identify the reflections by their direction.
     *
     * The group of transformations of the square is a non-abelian group defined as follows:
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
    enum class Transformation {
        IDENTITY = 0,
        ROTATION_BY_90,
        ROTATION_BY_180,
        ROTATION_BY_270,
        REFLECTION_IN_Y,
        REFLECTION_IN_X,
        REFLECTION_IN_NESW,
        REFLECTION_IN_NWSE,
    };

    /// A list of all Transformations for iteration.
    const std::vector<Transformation> transformations() noexcept;

    /// Function to serialize a Transformation.
    template <typename Archive>
    void serialize(Archive &ar, Transformation &d, const unsigned int version) {
        ar & d;
    }

    /// Compose two transformations.
    /**
     * This function, given two transformations, returns their composition.
     * @param t1 the first transformation to apply
     * @param t2 the second transformation to apply
     * @return the composition of applying t1 followed by t2
     */
    Transformation composeTransformations(Transformation t1, Transformation t2) noexcept;

    /// Get a human-readable representation of a transformation.
    std::string transformationName(Transformation t) noexcept;

    /// Determine the effects of a direction under the action of a transformation.
    /**
     * Given a @see{Direction}, determine the effects that the given transformation has on the
     * direction. (For example, a rotation by 90 degrees clockwise would take north and move it
     * to become east.)
     * @param t the transformation
     * @param d the direction
     * @return the new direction under the effect of the transformation
     */
    Direction applyTransformationToDirection(Transformation s, Direction d) noexcept;

    /// Determine the effects of a transformation on the dimensions.
    const Dimensions2D applyTransformationToDimensions(Transformation s, const Dimensions2D &d) noexcept;
}

BOOST_CLASS_VERSION(spelunker::types::Transformation, 1)

namespace spelunker::typeclasses {
    template<>
    struct Show<types::Transformation > {
        static std::string show(const types::Transformation s) {
            return types::transformationName(s);
        }
        static constexpr bool is_instance = true;
        using type = types::Transformation;
    };
}