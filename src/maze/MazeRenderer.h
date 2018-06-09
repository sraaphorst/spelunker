/**
 * MazeRenderer.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

namespace spelunker::maze {
    class Maze;

    class MazeRenderer {
    public:
        MazeRenderer() = default;
        virtual ~MazeRenderer() = default;

        virtual void render(const Maze &m) = 0;
    };
}
