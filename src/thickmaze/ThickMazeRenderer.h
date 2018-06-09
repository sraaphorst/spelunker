/**
 * ThickMazeRenderer.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

namespace spelunker::thickmaze {
    class ThickMaze;

    class ThickMazeRenderer {
    public:
        ThickMazeRenderer() = default;
        virtual ~ThickMazeRenderer() = default;

        virtual void render(const ThickMaze &m) = 0;
    };
}
