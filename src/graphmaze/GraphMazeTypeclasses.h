/**
 * GraphMazeTypeclasses.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <stdexcept>

#include <boost/graph/adjacency_list.hpp>

#include "typeclasses/Homomorphism.h"
#include "typeclasses/Show.h"
#include "maze/Maze.h"
#include "maze/MazeAttributes.h"
#include "GraphMaze.h"
#include "GraphMazeAttributes.h"

namespace spelunker::typeclasses {
    template<>
    struct Homomorphism<graphmaze::GraphMaze, maze::Maze> {
        static const maze::Maze morph(const graphmaze::GraphMaze &m) {
            const auto w = m.getWidth();
            const auto h = m.getHeight();

            // Start with all walls, and remove them as we iterate over the paths.
            auto numWalls = maze::calculateNumWalls(w, h);
            maze::WallIncidence wi(numWalls, true);

            auto lookup = m.getCellLookup();
            auto [eit, eend] = boost::edges(m.getUnderlyingGraph());
            for (; eit != eend; ++eit) {
                auto e = *eit;
                auto v1 = e.m_source;
                auto v2 = e.m_target;
                auto [x1,y1] = lookup[v1];
                auto [x2,y2] = lookup[v2];
                auto d = types::cellDirection(types::cell(x1,y1), types::cell(x2,y2));

                // Now carve the corresponding wall out of wc.
                auto wallID = maze::Maze::rankPositionS(w, h, x1, y1, d);
                wi[wallID] = false;
            }

            return maze::Maze(w, h, m.getStartingCell(), m.getEndingCells(), wi);
        }
    };

    template<>
    struct Show<graphmaze::GraphMaze> {
        static std::string show(const graphmaze::GraphMaze &gm) {
            auto m = Homomorphism<graphmaze::GraphMaze, maze::Maze>::morph(gm);
            return Show<maze::Maze>::show(m);
        }
    };
}