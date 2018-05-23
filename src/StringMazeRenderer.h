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
#include <string>
#include <vector>

#include "MazeAttributes.h"
#include "MazeRenderer.h"

namespace spelunker::maze {
    class Maze;

    /// A simple maze renderer to an ostream. We use this to define the Show type class for Maze.
    /**
     * This class takes an x by y Maze and converts it to a string that displays the maze using unicode box
     * characters. The final dimensions of the box output are (2x+1) by (y+1).
     */
    class StringMazeRenderer final : public MazeRenderer {
    public:
        StringMazeRenderer(std::ostream &o);
        virtual ~StringMazeRenderer() = default;

        virtual void render(const Maze &m) override;

    private:
        std::ostream &out;

        /// A quick and dirty extractor to get a wall status from a Maze, returning false for illegal coordinates.
        static bool wall(const Maze &m, int x, int y, types::Direction d);

        /// The characters used in the box form of the maze representation. There has to be a better way to do this.
        static const std::vector<std::string> boxchars;
    };
}


#endif //SPELUNKER_ASCIIMAZERENDERER_H
