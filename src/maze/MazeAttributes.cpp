/**
 * MazeAttributes.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <types/CommonMazeAttributes.h>
#include <types/Dimensions2D.h>

#include "MazeAttributes.h"

namespace spelunker::maze {
    const int calculateNumWalls(const int width, const int height) {
        return calculateNumWalls(types::Dimensions2D{width, height});
    }

    const int calculateNumWalls(const types::Dimensions2D &d) noexcept {
        const auto [width, height] = d.values();
        return (width - 1) * height + width * (height - 1);
    }

    WallIncidence createMazeLayout(int width, int height, bool walls) {
        // This involves just setting the wall incidences to true or false.
        return WallIncidence(calculateNumWalls(width, height), walls);
    }

    WallIncidence createMazeLayout(const types::Dimensions2D & d, bool walls) noexcept {
        return WallIncidence(calculateNumWalls(d), walls);
    }
}