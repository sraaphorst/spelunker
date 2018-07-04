/**
 * GraphMaze.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * A representation of a thin maze using Boost's graph library.
 */

#pragma once

#include <boost/graph/adjacency_list.hpp>

#include <iostream>

#include <types/AbstractMaze.h>
#include <types/BraidableMaze.h>
#include <types/CommonMazeAttributes.h>
#include <types/Dimensions2D.h>
#include <types/Direction.h>
#include <types/TransformableMaze.h>
#include <types/Transformation.h>
#include <types/UnicursalizableMaze.h>

#include "GraphMazeAttributes.h"

namespace spelunker::graphmaze {
    /**
     * A representation of thin Maze using Boost's graph library isomorphic to Maze.
     *
     * Note that whereas in Maze, we store a collection of walls, in the graph representation
     * of a maze, we have one vertex per maze cell and edges represent the absence of a wall
     * between two cells, i.e. a path.
     */
    class GraphMaze : public types::AbstractMaze,
                      public types::UnicursalizableMaze<GraphMaze>,
                      public types::BraidableMaze<GraphMaze>,
                      public types::TransformableMaze<GraphMaze> {
    public:
        /// Create a graph maze bounded by the dimensions given, with a start and end position.
        GraphMaze(const types::Dimensions2D &d,
                  const types::PossibleCell &start,
                  const types::CellCollection &goals,
                  const VertexCellPathCollection &ps);

        /// Create a graph maze bounded by width and height, with a start and end position.
        GraphMaze(int w,
                  int h,
                  const types::PossibleCell &s,
                  const types::CellCollection &ends,
                  const VertexCellPathCollection &ps);

        /// Create a maze bounded by the dimensions given.
        GraphMaze(const types::Dimensions2D &d,
                  const VertexCellPathCollection &ps);

        /// Create a maze bounded by width and height.
        GraphMaze(int w,
                  int h,
                  const VertexCellPathCollection &ps);

        ~GraphMaze() override = default;

        /// WARNING: This operation uses graph isomorphism checking, and thus may be VERY SLOW.
        bool operator==(const GraphMaze &other) const noexcept;

        inline const GridGraph &getUnderlyingGraph() const noexcept { return graph; }

        inline const CellFromVertexCellMap &getCellLookup() const noexcept { return lookup; }

        int numCellWalls(const types::Cell &c) const override;

        const GraphMaze applyTransformation(types::Transformation s) const override;

        const GraphMaze makeUnicursal() const noexcept override;

        const GraphMaze braid(double probability) const noexcept override;

        static GraphMaze load(std::istream &s);

        void save(std::ostream &s) const;

        const types::CellCollection neighbours(const types::Cell &c) const override;

    private:
        GraphMaze() = default;

        friend class boost::serialization::access;

        template<typename Archive>
        void serialize(Archive &ar, const unsigned int version);

        static GridGraph graphFromPaths(vertex_size_t numcells, const VertexCellPathCollection &ps);

        const GridGraph graph;

        // The matrix of vertices: maps (row,col) to vertex.
        const VertexCellGrid vertices;

        // The reverse lookup: maps vertex to (row,col).
        const CellFromVertexCellMap lookup;
    };
}

BOOST_CLASS_VERSION(spelunker::graphmaze::GraphMaze, 1)
