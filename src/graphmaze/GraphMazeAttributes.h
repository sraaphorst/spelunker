/**
 * GraphMazeAttributes.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Attributes used by GraphMazes.
 */

#pragma once

// We use Boost optional instead of STL optional for serialization purposes.
#include <boost/optional.hpp>

#include <tuple>
#include <vector>

#include <boost/graph/adjacency_list.hpp>

#include <types/Exceptions.h>
#include <types/CommonMazeAttributes.h>

namespace spelunker::graphmaze {
    /// The representation of a cell, which is a vertex in the graph.
    using VertexCell = int;

    /// A possible cell, used to represent, for example, the start vertex.
    using PossibleVertexCell = boost::optional<VertexCell>;

    /// A collection or row of VertexCells.
    using VertexCellCollection = std::vector<VertexCell>;

    /// A matrix of VertexCells, used to represent the grid that is a graph's cells.
    using VertexCellGrid = std::vector<VertexCellCollection>;

    /// A reverse lookup table, to map graph vertices to cell positions.
    using CellFromVertexCellMap = std::vector<types::Cell>;

    /// A valid path in the maze, from one cell to another.
    using VertexCellPath = std::pair<VertexCell, VertexCell>;

    /// A collection of paths, used as graph edges in a GraphMaze.
    using VertexCellPathCollection = std::vector<VertexCellPath>;

    /// The type of a grid graph, which is a simple graph.
    using GridGraph = boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS>;

    /// The type of the number of vertices in a graph.
    using vertex_size_t = GridGraph::vertices_size_type;

    /// Creates a matrix of VertexCells, increasing along rows.
    /**
     * Creates a matrix of VertexCells, increasing along rows. Paths in a GridGraph
     * should respect this pattern for consistency.
     * @param width width of the grid
     * @param height height of the grid
     * @return an initialized grid with position (0,0) = 0 and increasing along rows
     */
    inline const VertexCellGrid createVertexCellGrid(int width, int height) {
        if (width <= 0 || height <= 0)
            throw types::IllegalDimensions(width, height);

        VertexCellGrid m(height);
        auto v = 0;
        for (auto y=0; y < height; ++y) {
            VertexCellCollection row(width);
            for (auto x=0; x < width; ++x)
                row[x] = v++;
            m[y] = row;
        }
        return m;
    }

    /**
     * Creates a reverse lookup to createVertexCellGrid, i.e. takes a vertex number
     * and returns the pair (row,col) in which the vertex appears in the grid.
     * @param width the width of the grid
     * @param height the height of the grid
     * @return
     */
    inline const CellFromVertexCellMap createCellLookup(int width, int height) {
        if (width <= 0 || height <= 0)
            throw types::IllegalDimensions(width, height);

        CellFromVertexCellMap m(width * height);
        auto v = 0;
        for (VertexCell y=0; y < height; ++y)
            for (VertexCell x=0; x < width; ++x)
                m[v++] = std::make_pair(x, y);

        return m;
    }
};