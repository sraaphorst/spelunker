/**
 * ASCIIMazeRenderer.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Rendering code adapted from Vidar Holden's generatemaze.py script:
 * https://www.vidarholen.net/~vidar/generatemaze.py
 */

#ifndef SPELUNKER_ASCIIMAZERENDERER_H
#define SPELUNKER_ASCIIMAZERENDERER_H

#include <ostream>

#include "MazeRenderer.h"

namespace vorpal::maze {
    class Maze;

    /// A simple maze renderer to an ostream using ASCII. We use this to define the Show type class for Maze.
    class ASCIIMazeRenderer final : public MazeRenderer {
    public:
        ASCIIMazeRenderer(std::ostream &o);
        virtual ~ASCIIMazeRenderer() = default;

        virtual void render(const Maze &m) override;

    private:
        std::ostream &out;

        /// The characters used in the maze representation. There has to be a better way to do this.
        static constexpr auto chars = { " ", "\u2576", "\u2577", "\u250c", "\u2574", "\u2500", "\u2510", "\u252c",
                                        "\u2575", "\u2514", "\u2502", "\u251c", "\u2518", "\u2534", "\u2524", "\u253c" };
    };
}


#endif //SPELUNKER_ASCIIMAZERENDERER_H
