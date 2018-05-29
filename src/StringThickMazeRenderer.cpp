/**
 * StringThickMazeRenderer.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <ostream>
#include <string>

#include "StringThickMazeRenderer.h"
#include "ThickMaze.h"
#include "ThickMazeAttributes.h"

namespace spelunker::thickmaze {
    StringThickMazeRenderer::StringThickMazeRenderer(std::ostream &o) : out(o) {}

    void StringThickMazeRenderer::render(const spelunker::thickmaze::ThickMaze &tm) {
        const int width  = tm.getWidth();
        const int height = tm.getHeight();

        // Draw the border, and then the contents.
        // Hence, we iterate from -1 (the west and north border walls) to
        // width / height (the east / south border walls).
        for (auto y = -1; y <= height; ++y) {
            for (auto x = -1; x <= width; ++x) {
                if (x == -1 || x == width
                    || y == -1 || y == height
                    || tm.cellIs(x, y) == WALL)
                    out << "██";
                else out << "  ";

            }
            out << std::endl;
        }
    }
}
