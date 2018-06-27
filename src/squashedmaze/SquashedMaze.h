/**
 * SquashedMaze.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * A squashed maze is a representation of a maze as a graph that maintains many of its fundamental characteristics.
 */

#pragma once

#include <algorithm>
#include <map>
#include <queue>
#include <stdexcept>

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
                        const auto[x, y] = deadEnds.back();
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
                    const auto[dx, dy] = deadEnd;
                    ci[dx][dy] = true;
                }

                EdgeStart edgeStart = edgeQueue.front();
                edgeQueue.pop();

                // Mark this cell as visited.
                const auto &curCell = edgeStart.cells.back();
                const auto[curX, curY] = curCell;
                bool wasVisited = ci[curX][curY];
                ci[curX][curY] = true;

                // Get the neighbours of the last cell in the queue, and remove all vertices that
                // have already been visited.
                const types::CellCollection nbrs = m.neighbours(edgeStart.cells.back());
                types::CellCollection unvisitedNbrs;
                for (const types::Cell &c: nbrs) {
                    const auto[cx, cy] = c;
                    if (!(ci[cx][cy]))
                        unvisitedNbrs.emplace_back(c);
                }

                // We act depending on the number of neighbours.
                // If we have 0 neighbours, then we are an isolated cell.
                // If we have 1 neighbour and 0 unvisited neighbours, we are at the end of a passage.
                // If we have 2 neighbours, we are in a passage.
                // If we have 3 neighbours, we are at a T intersection.
                // If we have 4 neighbours, we are at a + intersection.
                // In all cases but 2, we want to create an edge.
                if (nbrs.size() == 0) {
                    // Single isolated cell: close off.
                    const auto[e, success] = boost::add_edge(edgeStart.u, edgeStart.u, 0, graph);
                    edges[e] = edgeStart.cells;

                    cout << "1. Adding edge " << e << " with weight " << *((int*)e.m_eproperty) << " and cells:" << endl << "\t";
                    for (const auto c: edgeStart.cells) { cout << "(" << c.first << "," << c.second << ") "; }cout << endl;

                } else if (nbrs.size() == 1 && unvisitedNbrs.size() == 0) {
                    // End of a passage: add a final vertex and close off.
                    const auto v = boost::add_vertex(graph);
                    vertexCell[curCell] = v;
                    const auto [e, success] = boost::add_edge(edgeStart.u, v, edgeStart.cells.size() - 1, graph);
                    edges[e] = edgeStart.cells;

                    cout << "2. Adding edge " << e << " with weight " << *((int*)e.m_eproperty) << " and cells:" << endl << "\t";
                    for (const auto c: edgeStart.cells) { cout << "(" << c.first << "," << c.second << ") "; }cout << endl;

                } else if ((nbrs.size() == 1 && unvisitedNbrs.size() == 1)
                        || (nbrs.size() == 2 && unvisitedNbrs.size() == 1)) {
                    // This covers two cases:
                    // 1. nbrs=1: We are at the very start of a passage.
                    // 2. nbrs=2: We are in the middle of a passage.
                    // In either case, extend with the unvisited cell and enqueue.
                    edgeStart.cells.emplace_back(unvisitedNbrs[0]);
                    edgeQueue.emplace(edgeStart);

                } else if (nbrs.size() == 2 && unvisitedNbrs.size() == 0) {
                    // In this case, we are closing a loop.
                    // Get the cell the loop closing cell ahead of us.
                    const types::Cell &lastVisited = edgeStart.cells.back();
                    const types::Cell &c = (lastVisited == nbrs[0]) ? nbrs[1] : nbrs[0];
                    edgeStart.cells.emplace_back(c);
                    const auto v = vertexCell[c];

                    // Make sure this edge doesn't exist already: otherwise we could have gone around a loop twice!
                    // We need .second since .first is the edge and .second is its existence.
                    if (!boost::edge(edgeStart.u, v, graph).second) {
                        const auto[e, success] = boost::add_edge(edgeStart.u, v, edgeStart.cells.size() - 1, graph);

                        cout << "3. Adding edge " << e << " with weight " << *((int *) e.m_eproperty) << " and cells:" << endl << "\t";
                        for (const auto c: edgeStart.cells) { cout << "(" << c.first << "," << c.second << ") "; } cout << endl;
                    }

                } else if (nbrs.size() == unvisitedNbrs.size()) {
                    // ERROR CONDITION: This can never happen.
                    throw std::domain_error("Unexpected maze configuration.");

                } else {
                    // We are at a junction. Check to see if a vertex exists for this cell, and if not, create one.
                    const auto iter = vertexCell.find(curCell);
                    const auto v = (iter == vertexCell.end()) ? boost::add_vertex(graph) : iter->second;
                    if (iter == vertexCell.end())
                        vertexCell[curCell] = v;

                    // Now end this edge, and create new edges for all the unvisited neighbours.
                    const auto [e, success] = boost::add_edge(edgeStart.u, v, edgeStart.cells.size() - 1, graph);
                    edges[e] = edgeStart.cells;

                    cout << "4s. Adding edge " << e << " with weight " << *((int*)e.m_eproperty) << " and cells:" << endl << "\t";
                    for (const auto c: edgeStart.cells) { cout << "(" << c.first << "," << c.second << ") "; }cout << endl;

                    // Create new EdgeStarts for each unvisited neighbour and enqueue them.
                    for (const auto &n: unvisitedNbrs)
                        edgeQueue.emplace(EdgeStart{v, types::CellCollection{curCell, n}});
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

