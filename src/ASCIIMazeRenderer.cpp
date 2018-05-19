/**
 * ASCIIMazeRenderer.cpp
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Rendering code adapted from Vidar Holden's generatemaze.py script:
 * https://www.vidarholen.net/~vidar/generatemaze.py
 */

#include <ostream>

#include "ASCIIMazeRenderer.h"
#include "MazeAttributes.h

namespace vorpal::maze {
    ASCIIMazeRenderer::ASCIIMazeRenderer(std::ostream &o) : out(o) {}

    void ASCIIMazeRenderer::render(const vorpal::maze::Maze &m) {
        const auto w = m.getWidth() - 1;
        const auto h = m.getHeight() - 1;

        for (auto y = 0; y < h; ++y) {
            for (auto x = 0; x < w; ++x) {
                const auto n = m.wall(x,     y, types::NORTH) ? 0 : 1;
                const auto w = m.wall(x,     y, types::WEST)  ? 0 : 1;
                const auto s = m.wall(x,   y+1, types::NORTH) ? 0 : 1;
                const auto e = m.wall(x+1,   y, types::WEST)  ? 0 : 1;

                const auto idx = n * 8 + w * 4 + s * 2 + e;
                out << chars[idx] << chars[idx == 0 ? 0 : 5];
            }
            out << std::endl;
        }
    }
}