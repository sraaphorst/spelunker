/**
 * ASCIIMazeRenderer.cpp
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Rendering code adapted from Vidar Holden's generatemaze.py script:
 * https://www.vidarholen.net/~vidar/generatemaze.py
 */

#include <ostream>
#include <string>

#include "StringMazeRenderer.h"
#include "Maze.h"
#include "MazeAttributes.h"

namespace spelunker::maze {
    const std::vector<std::string> StringMazeRenderer::boxchars = { " ","╶","╷","┌","╴","─","┐","┬","╵","└","│","├","┘","┴","┤","┼" };

    StringMazeRenderer::StringMazeRenderer(std::ostream &o) : out(o) {}

    bool StringMazeRenderer::wall(const Maze &m, int x, int y, spelunker::maze::types::Direction d) {
        if (x < 0 || x >= m.getWidth())  return false;
        if (y < 0 || y >= m.getHeight()) return false;
        return m.wall(x, y, d);
    }

    void StringMazeRenderer::render(const spelunker::maze::Maze &m) {
        /**
         * First, we have to convert the Maze from an x by y grid into an (x+1) by (y+1) box drawing.
         * To determine cell (x,y) in the box drawing, we need the following cells from the Maze:
         * (x-1,y-1), (x-1,y), (x,y-1), (x,y)
         *
         * Define two functions:
         * B(x,y,d) = true iff direction d is part of the symbol for box coordinate (x,y).
         * M(x,y,d) = true iff (x,y) is in bounds and cell (x,y) has a wall at position d.
         *
         * Then we define B(x,y,d) in terms of M(x,y,d) as follows:
         * 1. B(x,y,N) = M(x-1, y-1, E) || M(  x, y-1, W)
         * 2. B(x,y,W) = M(x-1, y-1, S) || M(x-1,   y, N)
         * 3. B(x,y,S) = M(x-1, y,   E) || M(  x,   y, W)
         * 4. B(x,y,E) = M(  x, y-1, S) || M(  x,   y, N)
         */
        const auto width  = m.getWidth();
        const auto height = m.getHeight();

        // Create the box representation of the maze.
        using BoxEntry = std::vector<bool>;
        using BoxColumn = std::vector<BoxEntry>;
        using BoxRepresentation = std::vector<BoxColumn>;

        BoxEntry          bDir(4, false);
        BoxColumn         bCol(height+1, bDir);
        BoxRepresentation box(width+1, bCol);

        for (auto x=0; x <= width; ++x) {
            for (auto y=0; y <= height; ++y) {
                box[x][y][types::NORTH] = wall(m, x-1, y-1, types::EAST)  || wall(m,   x, y-1, types::WEST);
                box[x][y][types::WEST]  = wall(m, x-1, y-1, types::SOUTH) || wall(m, x-1,   y, types::NORTH);
                box[x][y][types::SOUTH] = wall(m, x-1,   y, types::EAST)  || wall(m,   x,   y, types::WEST);
                box[x][y][types::EAST]  = wall(m,   x, y-1, types::SOUTH) || wall(m,   x,   y, types::NORTH);
            }
        }

        // Now we are ready to render.
        for (auto y=0; y <= height; ++y) {
            for (auto x=0; x <= width; ++x) {
                const auto n = box[x][y][types::NORTH] ? 1 : 0;
                const auto w = box[x][y][types::WEST]  ? 1 : 0;
                const auto s = box[x][y][types::SOUTH] ? 1 : 0;
                const auto e = box[x][y][types::EAST]  ? 1 : 0;

                const auto idx = n * 8 + w * 4 + s * 2 + e;
                out << boxchars[idx] << boxchars[e == 0 ? 0 : 5];
            }
            out << std::endl;
        }
    }
}