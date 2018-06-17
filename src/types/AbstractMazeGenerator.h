/**
 * AbstractMazeGenerator.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include "Dimensions2D.h"

namespace spelunker::types {
    template<typename T>
    class AbstractMazeGenerator {
    protected:
        AbstractMazeGenerator(const types::Dimensions2D &d);
        AbstractMazeGenerator(int w, int h);

        virtual ~AbstractMazeGenerator() = default;

    public:
        inline const Dimensions2D &getDimensions() const noexcept {
            return dimensions;
        }

        inline const int getWidth() const noexcept {
            return dimensions.getWidth();
        }

        inline const int getHeight() const noexcept {
            return dimensions.getHeight();
        }

        virtual const T generate() const noexcept = 0;

    private:
        const Dimensions2D dimensions;
    };
}


