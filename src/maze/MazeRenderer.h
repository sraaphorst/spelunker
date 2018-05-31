/**
 * MazeRenderer.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#ifndef SPELUNKER_MAZERENDERER_H
#define SPELUNKER_MAZERENDERER_H

namespace spelunker::maze {
    class Maze;

    class MazeRenderer {
    public:
        MazeRenderer() = default;
        virtual ~MazeRenderer() = default;

        virtual void render(const Maze &m) = 0;
    };
}

#endif //SPELUNKER_MAZERENDERER_H
