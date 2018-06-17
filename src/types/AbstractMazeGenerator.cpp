/**
 * AbstractMazeGenerator.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include "AbstractMazeGenerator.h"
#include "Dimensions2D.h"

namespace spelunker::types {
    AbstractMazeGenerator::AbstractMazeGenerator(const spelunker::types::Dimensions2D &d)
        : dimensions{d} {}

    AbstractMazeGenerator::AbstractMazeGenerator(int w, int h)
        : dimensions{Dimensions2D{w, h}} {}
}