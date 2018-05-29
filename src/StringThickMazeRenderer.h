/**
 * StringThickMazeRenderer.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#ifndef SPELUNKER_STRINGTHICKMAZERENDERER_H
#define SPELUNKER_STRINGTHICKMAZERENDERER_H

#include <ostream>
#include <string>

#include "ThickMazeRenderer.h"

namespace spelunker::thickmaze {

    /// A simple maze renderer to an ostream. We use this to define the Show type class for ThickMaze.
    /**
     * This class takes an x by y ThickMaze and converts it to a string that displays the maze using
     * unicode characters.
     */
    class StringThickMazeRenderer final : public ThickMazeRenderer {
    public:
        explicit StringThickMazeRenderer(std::ostream &o);
        ~StringThickMazeRenderer() final = default;

        void render(const ThickMaze &tm) final;

    private:
        std::ostream &out;
    };
}

#endif //SPELUNKER_STRINGTHICKMAZERENDERER_H
