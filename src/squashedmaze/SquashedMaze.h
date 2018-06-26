/**
 * SquashedMaze.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * A squashed maze is a representation of a maze as a graph that maintains many of its fundamental characteristics.
 */

#pragma once

#include <map>
#include <queue>

#include <boost/graph/adjacency_list.hpp>

#include <maze/Maze.h>
#include <types/CommonMazeAttributes.h>
#include <types/AbstractMaze.h>
#include "SquashedMazeAttributes.h"

#include <iostream>
using namespace std;

namespace spelunker::squashedmaze {
    /**
     * A SquashedMaze is a minimal representation of a maze as a graph for statistical and algorithmic purposes.
     *
     * Given an AbstractMaze, the squashed maze has one vertex for:
     * 1. Each dead end in the original maze;
     * 2. Each junction (or decision point) in the original maze;
     * 3. An independent vertex for any isolated loops; and
     * 4. Edges weighted by the number of cells they cover.
     *
     * Each edge covers a number of cells in the initial graph: we maintain this information as well. Some cells
     * appear in multiple edges, i.e. the cells at junctures appear in the 3-4 edges they represent.
     *
     * Note that a SquashedMaze does not uniquely represent a maze, i.e. multiple mazes may generate the same
     * SquashedMaze. Like a hashcode, however, if two mazes generate different SquashedMazes, they are not equal.
     */
    class SquashedMaze {
    public:
        template<typename T>
        SquashedMaze(const types::AbstractMaze<T> &m) {
            // Make sure that every cell is covered.
            const auto width  = m.getWidth();
            const auto height = m.getHeight();
            auto ci = types::initializeCellIndicator(m.getDimensions());

            // Get a list of the dead ends. We start by covering dead ends and all that is left after
            // that are loops.
            auto deadEnds = m.findDeadEnds();

            // Create the queue, which comprises a starting vertex, and a list of the covered cells.
            struct EdgeStart {
                const WeightedGraphVertex u;
                types::CellCollection cells;
            };
            std::queue<EdgeStart> edgeQueue;

            while (!deadEnds.empty()) {
                // If the queue is empty, make a new start at a dead end for it.
                if (edgeQueue.empty()) {
                    // Keep popping dead ends until we find an unvisited one.
                    while (!deadEnds.empty()) {
                        const auto [x, y] = deadEnds.back();
                        if (ci[x][y])
                            deadEnds.pop_back();
                        else
                            break;
                    }

                    // If we are out of dead ends, then we are done.
                    if (deadEnds.empty())
                        break;

                    const auto deadEnd = deadEnds.back();
                    deadEnds.pop_back();

                    const WeightedGraphVertex u = boost::add_vertex(graph);
                    vertexCell[deadEnd] = u;
                    edgeQueue.emplace(EdgeStart{u, types::CellCollection{deadEnd}});

                    // Mark the cell as visited.
                    const auto [dx, dy] = deadEnd;
                    ci[dx][dy] = true;
                }

                EdgeStart edgeStart = edgeQueue.front();
                edgeQueue.pop();

                // Mark this cell as visited.
                const auto &curCell = edgeStart.cells.back();
                const auto [curX, curY] = curCell;
                ci[curX][curY] = true;

                // Get the neighbours of the last cell in the queue, and remove all vertices that
                // have already been visited.
                const types::CellCollection nbrs0 = m.neighbours(edgeStart.cells.back());
                types::CellCollection unvisitedNbrs;
                for (const types::Cell &c: nbrs0) {
                    const auto [cx, cy] = c;
                    if (!ci[cx][cy])
                        unvisitedNbrs.emplace_back(c);
                }

                // We act depending on the number of neighbours.
                // If we have 0, 2, or 3 neighbours, we are at the end of this edge.
                // Close the edge up and add it to our edge list.
                if (unvisitedNbrs.size() == 0 || unvisitedNbrs.size() == 2 || unvisitedNbrs.size() == 3) {
                    const auto v = boost::add_vertex(graph);
                    vertexCell[curCell] = v;

                    const auto[e, success] = boost::add_edge(edgeStart.u, v, edgeStart.cells.size() - 1, graph);
                    edges[e] = edgeStart.cells;
                    cout << "Adding edge " << e << " for vertices " << edgeStart.u << " and " << v << " with weight " << *((int*)e.m_eproperty) << " and cells:"
                         << endl << "\t";
                    for (const auto c: edgeStart.cells) {
                        cout << "(" << c.first << "," << c.second << ") ";
                    }
                    cout << endl;

                    // If there are unvisited neighbours, we are at a T juncture or a + juncture.
                    // Create new EdgeStarts for each unvisited neighbour and enqueue them.
                    for (const auto &n: unvisitedNbrs) {
                        edgeQueue.emplace(EdgeStart{v, types::CellCollection{curCell, n}});
                    }
                } else if (unvisitedNbrs.size() == 1) {
                    // If we have one, we are in a passage that is ongoing.
                    // Add the neighbour to the cell, and enqueue it again.
                    edgeStart.cells.emplace_back(unvisitedNbrs[0]);
                    edgeQueue.emplace(edgeStart);
                }
            }

            // Now we have taken care of all the dead ends: all that is left are possible loops.
            // Iterate over the cell coverage and perform a BFS on each uncovered cell.
            for (auto y = 0; y < height; ++y) {
                for (auto x = 0; x < width; ++x) {
                    if (ci[x][y]) continue;
                    if (!m.cellInBounds(x, y)) {
                        ci[x][y] = true;
                        continue;
                    }

                    // Perform a BFS starting here.
                    const types::BFSResults bfsResults = m.performBFSFrom(types::cell(x, y));
                    const types::CellCollection &loop = bfsResults.connectedCells;

                    // Create a vertex and loop in the graph with these cells.
                    const auto v = boost::add_vertex(graph);
                    vertexCell[types::cell(x, y)] = v;
                    const auto [e, success] = boost::add_edge(v, v, 0, graph);

                    // Mark all the cells as visited and add a disconnected vertex in a loop to the graph.
                    edges[e] = loop;
                }
            }
        };

        ~SquashedMaze() = default;

        using EdgeCellMap = std::map<WeightedGraphEdge, types::CellCollection>;
        const EdgeCellMap &getMap() const noexcept {
            return edges;
        }

        using CellVertexMap = std::map<types::Cell, WeightedGraphVertex>;

        const WeightedGraph &getGraph() const noexcept {
            return graph;
        }

    private:
        // Each edge covers multiple cells. We map between cells and edges.
        EdgeCellMap edges;

        // Each vertex of the squashed maze is associated with a cell in the original maze.
        CellVertexMap vertexCell;

        // The graph that represents the squashed maze.
        WeightedGraph graph;
    };
}

