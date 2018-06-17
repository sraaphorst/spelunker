/**
 * MazeTypeclasses.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <sstream>
#include <string>
#include <vector>

#include "types/CommonMazeAttributes.h"
#include "typeclasses/Homomorphism.h"
#include "typeclasses/Show.h"
#include "thickmaze/ThickMaze.h"
#include "thickmaze/ThickMazeAttributes.h"
#include "graphmaze/GraphMaze.h"
#include "graphmaze/GraphMazeAttributes.h"
#include "Maze.h"
#include "MazeAttributes.h"
#include "StringMazeRenderer.h"

namespace spelunker::typeclasses {
    template<>
    struct Show<maze::Maze> {
        static std::string show(const maze::Maze &m) {
            std::ostringstream out;
            maze::StringMazeRenderer r(out);
            r.render(m);
            return out.str();
        }

        static constexpr bool is_instance = true;
        using type = maze::Maze;
    };

    template<>
    struct Homomorphism<maze::Maze, thickmaze::ThickMaze> {
        static const thickmaze::ThickMaze morph(const maze::Maze &m) {
            const auto mwidth  = m.getWidth();
            const auto mheight = m.getHeight();
            const auto twidth  = 2 * mwidth - 1;
            const auto theight = 2 * mheight - 1;

            auto contents = thickmaze::createThickMazeCellContents(twidth, theight);

            // Iterate over the walls of the maze and add them to the thick maze, focusing
            // on the east and the south walls of maze.
            // For each maze wall, mark (provided not out of bounds) three wall segments
            // in the thick maze.
            for (auto x = 0; x < mwidth; ++x)
                for (auto y = 0; y < mheight; ++y) {
                    // Ignore the last row of maze when adding southern walls.
                    if (y < mheight-1 && m.wall(x, y, SOUTH)) {
                        // Find the central position in the thick maze.
                        const int cx = 2 * x;
                        const int cy = 2 * y + 1;
                        if (cx > 0) contents[cx-1][cy] = thickmaze::WALL;
                        contents[cx][cy] = thickmaze::WALL;
                        if (cx < twidth-1) contents[cx+1][cy] = thickmaze::WALL;
                    }

                    // Ignore the last row of maze when adding eastern walls.
                    if (x < mwidth - 1 && m.wall(x, y, EAST)) {
                        // Find the central position in the thick maze.
                        const int cx = 2 * x + 1;
                        const int cy = 2 * y;
                        if (cy > 0) contents[cx][cy-1] = thickmaze::WALL;
                        contents[cx][cy] = thickmaze::WALL;
                        if (cy < theight-1) contents[cx][cy+1] = thickmaze::WALL;
                    }
                }
            return thickmaze::ThickMaze(twidth, theight, contents);
        }

        static constexpr bool is_instance = true;
        using src  = maze::Maze;
        using dest = thickmaze::ThickMaze;
    };

    template<>
    struct Homomorphism<maze::Maze, graphmaze::GraphMaze> {
        static const graphmaze::GraphMaze morph(const maze::Maze &m) {
            const auto w = m.getWidth();
            const auto h = m.getHeight();
            graphmaze::VertexCellPathCollection vc;

            // Iterate over the walls of this maze and if they are carved out, add
            // them to the list of paths. Only deal with east and south walls since
            // other directions are just repeats of those two.
            const auto maxH = h - 1;
            const auto maxW = w - 1;
            int v = 0;
            for (auto y=0; y < h; ++y)
                for (auto x=0; x < w; ++x) {
                    if (!m.wall(types::pos(x, y, types::EAST)))
                        vc.emplace_back(v, v + 1);
                    if (!m.wall(types::pos(x, y, types::SOUTH)))
                        vc.emplace_back(v, v + w);
                    ++v;
                }

            return graphmaze::GraphMaze(w, h, m.getStartingCell(), m.getGoalCells(), vc);
        }

        static constexpr bool is_instance = true;
        using src  = maze::Maze;
        using dest = graphmaze::GraphMaze;
    };
}
