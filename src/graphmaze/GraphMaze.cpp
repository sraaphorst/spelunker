/**
 * GraphMaze.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <boost/optional.hpp>
#include <vector>

#include <boost/graph/adjacency_list.hpp>

#include <types/CommonMazeAttributes.h>
#include <types/Exceptions.h>
#include <typeclasses/Homomorphism.h>
#include <typeclasses/Show.h>
#include <maze/Maze.h>

#include "GraphMazeAttributes.h"
#include "GraphMaze.h"

namespace spelunker::graphmaze {
    GraphMaze::GraphMaze(int w,
                         int h,
                         const types::PossibleCell &s,
                         const types::CellCollection &ends,
                         const VertexCellPathCollection &ps)
            : width{w}, height{h}, startCell{s}, endingCells{ends},
              vertices{createVertexCellGrid(w, h)},
              lookup{createCellLookup(w, h)},
              graph{graphFromPaths(w*h, ps)} {}

    GraphMaze::GraphMaze(int w, int h, const spelunker::graphmaze::VertexCellPathCollection &ps)
        : GraphMaze{w, h, {}, types::CellCollection(), ps} {}


    GridGraph GraphMaze::graphFromPaths(const vertex_size_t numcells,
                                        const VertexCellPathCollection &ps) {
        for (auto iter = ps.begin(); iter != ps.end(); ++iter)
            auto [v1,v2] = *iter;
        return GridGraph{ps.begin(), ps.end(), numcells};
    }
}
