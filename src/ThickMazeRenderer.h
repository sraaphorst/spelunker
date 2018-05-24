/**
 * ThickMazeRenderer.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#ifndef SPELUNKER_THICKMAZERENDERER_H
#define SPELUNKER_THICKMAZERENDERER_H

namespace spelunker::thickmaze {
    class ThickMaze;

    class ThickMazeRenderer {
    public:
        virtual void render(const ThickMaze &m) = 0;
    };
}

#endif //SPELUNKER_THICKMAZERENDERER_H
