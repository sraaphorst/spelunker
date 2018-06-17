/**
 * GraphMaze.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * A representation of a thin maze using Boost's graph library.
 */

#pragma once

#include <types/CommonMazeAttributes.h>
#include "GraphMazeAttributes.h"
#include <boost/graph/adjacency_list.hpp>

namespace spelunker::graphmaze {
    /**
     * A representation of thin Maze using Boost's graph library isomorphic to Maze.
     *
     * Note that whereas in Maze, we store a collection of walls, in the graph representation
     * of a maze, we have one vertex per maze cell and edges represent the absence of a wall
     * between two cells, i.e. a path.
     */
    class GraphMaze final {
    public:
        /// Create a graph maze bounded by width and height, with a start and end position.
        GraphMaze(int w,
                  int h,
                  const types::PossibleCell &s,
                  const types::CellCollection &ends,
                  const VertexCellPathCollection &ps);

        /// Creates a maze bounded by width and height, but with no start / end positions.
        GraphMaze(int w,
                  int h,
                  const VertexCellPathCollection &ps);

        ~GraphMaze() = default;

        inline const int getWidth() const noexcept { return width; }
        inline const int getHeight() const noexcept { return height; }

        inline const GridGraph &getUnderlyingGraph() const noexcept { return graph; }

        inline const CellFromVertexCellMap getCellLookup() const noexcept { return lookup; }

        inline types::PossibleCell getStartingCell() const noexcept  { return startCell; }

        inline types::CellCollection getEndingCells() const noexcept { return endingCells; }

    private:
        static GridGraph graphFromPaths(vertex_size_t numcells, const VertexCellPathCollection &ps);

        const int width;
        const int height;

        const types::PossibleCell startCell;
        const types::CellCollection endingCells;

        const GridGraph graph;

        // The matrix of vertices: maps (row,col) to vertex.
        VertexCellGrid vertices;

        // The reverse lookup: maps vertex to (row,col).
        CellFromVertexCellMap lookup;
    };
}
