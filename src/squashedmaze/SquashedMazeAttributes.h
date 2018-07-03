/**
 * SquashedMazeAttributes.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <boost/graph/adjacency_list.hpp>

#include <types/CommonMazeAttributes.h>

namespace spelunker::squashedmaze {
    /// The type of a squashed maze graph, which is a simple graph.
    using WeightedGraph = boost::adjacency_list<boost::setS, boost::vecS, boost::undirectedS, boost::no_property, boost::property<boost::edge_weight_t, int>>;
    using WeightedGraphVertex = WeightedGraph::vertex_descriptor;
    using WeightedGraphEdge = WeightedGraph::edge_descriptor;

    using EdgeCellMap = std::map<WeightedGraphEdge, types::CellCollection>;
    using CellVertexMap = std::map<types::Cell, WeightedGraphVertex>;
}