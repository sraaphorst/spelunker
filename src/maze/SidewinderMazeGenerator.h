/**
 * SidewinderMazeGenerator.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Generate a maze using the
 * <a href="http://weblog.jamisbuck.org/2011/2/3/maze-generation-sidewinder-algorithm">
 * sidewinder algorithm.</a>
 */

#pragma once

#include <types/Dimensions2D.h>

#include "MazeGenerator.h"

namespace spelunker::maze {
    class Maze;

    class SidewinderMazeGenerator final : public MazeGenerator {
    public:
        SidewinderMazeGenerator(const types::Dimensions2D &d, double p);
        SidewinderMazeGenerator(int w, int h, double p);
        SidewinderMazeGenerator(const types::Dimensions2D &d);
        SidewinderMazeGenerator(int w, int h);

        ~SidewinderMazeGenerator() final = default;

        const Maze generate() const noexcept final;

        static constexpr double defaultProbabilityEast = 0.5;

    private:
        const double probabilityEast;
    };
}
