/**
 * Symmetry.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Symmetries that can be applied to mazes.
 */

#pragma once

#include <string>

#include <typeclasses/Show.h>

namespace spelunker::types {
    class Dimensions2D;

    /// Maze symmetries. Note that the diagonal reflections can only be performed for square mazes.
    /**
     * Maze symmetries. Since we work with (0,0) in the upper left corner, which is not standard in the euclidean
     * plane, we identify the reflections by their direction.
     */
    enum class Symmetry {
        ROTATION_BY_90 = 0,
        ROTATION_BY_180,
        ROTATION_BY_270,
        REFLECTION_IN_Y,
        REFLECTION_IN_X,
        REFLECTION_IN_NWSE,
        REFLECTION_IN_NESW,
    };

    /// Get a human-readable representation of a symmetry.
    std::string symmetryName(Symmetry s);

    /// Determine the effects of a direction under the action of a symmetry.
    /**
     * Given a @see{Direction}, determine the effects that the given symmetry has on the
     * direction. (For example, a rotation by 90 degrees clockwise would take north and move it
     * to become east.)
     * @param s the symmetry
     * @param d the direction
     * @return the new direction under the effect of the symmetry
     */
    Direction applySymmetryToDirection(Symmetry s, Direction d);

    /// Determine the effects of a symmetry on the dimensions.
    const Dimensions2D applySymmetryToDimensions(Symmetry s, const Dimensions2D &d);
}

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