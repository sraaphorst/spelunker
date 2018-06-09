/**
 * ThickMazeGenerator.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Superclass of algorithms unique to @see{ThickMaze}s.
 */

#pragma once

namespace spelunker::thickmaze {
    class ThickMaze;

    class ThickMazeGenerator {
    public:
        ThickMazeGenerator(int w, int h);
        virtual ~ThickMazeGenerator() = default;

        virtual const ThickMaze generate() = 0;

    protected:
        const int width;
        const int height;
    };
};
