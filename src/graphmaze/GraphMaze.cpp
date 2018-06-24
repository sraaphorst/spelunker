/**
 * GraphMaze.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <boost/optional.hpp>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adj_list_serialize.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <types/CommonMazeAttributes.h>
#include <types/Exceptions.h>
#include <typeclasses/Homomorphism.h>
#include <typeclasses/Show.h>
#include <maze/Maze.h>
#include <maze/MazeTypeclasses.h>

#include "GraphMazeAttributes.h"
#include "GraphMazeTypeclasses.h"
#include "GraphMaze.h"

namespace spelunker::graphmaze {
    GraphMaze::GraphMaze(const types::Dimensions2D &d,
                         const types::PossibleCell &start,
                         const types::CellCollection &goals,
                         const VertexCellPathCollection &ps)
         : types::AbstractMaze<GraphMaze>{d, start, goals},
           vertices{createVertexCellGrid(d)},
           lookup{createCellLookup(d)},
           graph{graphFromPaths(d.getWidth() * d.getHeight(), ps)} {}

    GraphMaze::GraphMaze(const int w,
                         const int h,
                         const types::PossibleCell &start,
                         const types::CellCollection &goals,
                         const VertexCellPathCollection &ps)
            : GraphMaze{types::Dimensions2D{w, h}, start, goals, ps} {}

    GraphMaze::GraphMaze(const types::Dimensions2D &d,
                         const VertexCellPathCollection &ps)
        : GraphMaze{d, {}, types::CellCollection{}, ps} {}

    GraphMaze::GraphMaze(const int w,
                         const int h,
                         const spelunker::graphmaze::VertexCellPathCollection &ps)
        : GraphMaze{types::Dimensions2D{w, h}, {}, types::CellCollection(), ps} {}

    GridGraph GraphMaze::graphFromPaths(const vertex_size_t numcells,
                                        const VertexCellPathCollection &ps) {
        return GridGraph{ps.begin(), ps.end(), numcells};
    }

    int GraphMaze::numCellWalls(const types::Cell &c) const {
        const auto [x, y] = c;
        const auto v = vertices[x][y];
        return graph.out_edge_list(v).size();
    }

    const GraphMaze GraphMaze::applySymmetry(types::Symmetry s) const {
        // Cheat by taking the homomorphic way out.
        const maze::Maze m = typeclasses::Homomorphism<GraphMaze, maze::Maze>::morph(*this);
        return typeclasses::Homomorphism<maze::Maze, GraphMaze>::morph(m.applySymmetry(s));
    }

    const GraphMaze GraphMaze::makeUnicursal() const {
        // Cheat by taking the homomorphic way out.
        const maze::Maze m = typeclasses::Homomorphism<GraphMaze, maze::Maze>::morph(*this);
        return typeclasses::Homomorphism<maze::Maze, GraphMaze>::morph(m.makeUnicursal());
    }

    const GraphMaze GraphMaze::braid(double probability) const noexcept {
        // Cheat by taking the homomorphic way out.
        const maze::Maze m = typeclasses::Homomorphism<GraphMaze, maze::Maze>::morph(*this);
        return typeclasses::Homomorphism<maze::Maze, GraphMaze>::morph(m.braid(probability));
    }

    GraphMaze GraphMaze::load(std::istream &s) {
        GraphMaze gm;
        boost::archive::text_iarchive ia{s};
        ia >> gm;
        return gm;
    }

    void GraphMaze::save(std::ostream &s) const {
        boost::archive::text_oarchive oa{s};
        oa << *this;
    }

    template<typename Archive>
    void GraphMaze::serialize(Archive &ar, const unsigned int version) {
        ar & boost::serialization::base_object<types::AbstractMaze<GraphMaze>>(*this);
        ar & const_cast<GridGraph&>(graph);
        ar & const_cast<VertexCellGrid&>(vertices);
        ar & const_cast<CellFromVertexCellMap&>(lookup);
    }

    const types::CellCollection GraphMaze::neighbours(const types::Cell &c) const {
        checkCell(c);
        const auto [x, y] = c;
        const auto v = vertices[x][y];

        types::CellCollection cc;
        auto [i, end] = boost::adjacent_vertices(v, graph);
        for (; i != end; ++i) {
            const auto w = *i;
            const auto [nx, ny] = lookup[w];
            cc.emplace_back(types::cell(nx, ny));
        }

        return cc;
    }
}
