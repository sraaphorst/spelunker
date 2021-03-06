/**
 * StringThickMazeRenderer.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <ostream>
#include <string>

#include "ThickMaze.h"
#include "ThickMazeAttributes.h"
#include "StringThickMazeRenderer.h"

namespace spelunker::thickmaze {
    StringThickMazeRenderer::StringThickMazeRenderer(std::ostream &o) : out(o) {}

    void StringThickMazeRenderer::render(const spelunker::thickmaze::ThickMaze &tm) {
        const auto [width, height] = tm.getDimensions().values();

        // Draw the border, and then the contents.
        // Hence, we iterate from -1 (the west and north border walls) to
        // width / height (the east / south border walls).
        for (auto y = -1; y <= height; ++y) {
            for (auto x = -1; x <= width; ++x) {
                if (x == -1 || x == width
                    || y == -1 || y == height
                    || tm.cellIs(x, y) == CellType::WALL)
                    out << "██";
                else out << "  ";

            }
            out << std::endl;
        }
    }
}
