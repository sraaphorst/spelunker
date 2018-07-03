/**
 * SquashedMaze.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * A squashed maze is a representation of a maze as a graph that maintains many of its fundamental characteristics.
 */

#pragma once

#include <cassert>
#include <algorithm>
#include <map>
#include <queue>
#include <stdexcept>

#include <boost/graph/adjacency_list.hpp>

#include <maze/Maze.h>
#include <types/CommonMazeAttributes.h>
#include <types/AbstractMaze.h>
#include <typeclasses/Show.h>
#include "RoomFinder.h"
#include "SquashedMazeAttributes.h"

#ifdef DEBUG
#include <iostream>
using namespace std;
#endif

namespace spelunker::squashedmaze {
    /**
     * A SquashedMaze is a minimal representation of a maze as a graph for statistical and algorithmic purposes.
     *
     * Given an AbstractMaze, the squashed maze has one vertex for:
     * 1. Each dead end in the original maze;
     * 2. Each junction (or decision point) in the original maze;
     * 3. Each "entrance cell" inside a room found by the RoomFinder; and
     * 4. Each loop in the graph.
     *
     * Edges are weighted by the number of cells they cover.
     *
     * Each edge covers a number of cells in the initial graph: we maintain this information as well. Some cells
     * appear in multiple edges, i.e. the cells at junctures appear in the 3-4 edges they represent.
     *
     * Note that a SquashedMaze does not uniquely represent a maze, i.e. multiple mazes may generate the same
     * SquashedMaze. Like a hashcode, however, if two mazes generate different SquashedMazes, they are not equal.
     *
     * This class also provides us with a number of statistics:
     * 1. Number of passages, as well as their length.
     * 2. Number of T and + junctions (which could have been easily ascertained before).
     * 3. Number of rooms and their size.
     */
    class SquashedMaze {
    public:
        template<typename T>
        SquashedMaze(const types::AbstractMaze<T> &m);

        ~SquashedMaze() = default;

        const EdgeCellMap &getMap() const noexcept {
            return edges;
        }

        const WeightedGraph &getGraph() const noexcept {
            return graph;
        }

    private:
        /**
         * Process a room as found by @see[RoomFinder].
         * For each room, we:
         * 1. Find the entrances, and add a vertex for each;
         * 2. Find the shortest path between each of the entrances and add an edge for each;
         * @tparam T the maze type
         * @param m the maze
         * @param cc the collection of cells comprising the room
         */
        template<typename T>
        void processRoom(const types::AbstractMaze<T> &m, const types::CellCollection &cc);

        // Each edge covers multiple cells. We map between cells and edges.
        EdgeCellMap edges;

        // Each vertex of the squashed maze is associated with a cell in the original maze.
        CellVertexMap vertexCell;

        // The graph that represents the squashed maze.
        WeightedGraph graph;
    };



    /****************************
     *** FUNCTION DEFINITIONS ***
     ****************************/

    template<typename T>
    SquashedMaze::SquashedMaze(const spelunker::types::AbstractMaze<T> &m) {
        /*** PREPROCESSING ***/
        // Make sure that every cell is covered.
        const auto width  = m.getWidth();
        const auto height = m.getHeight();
        auto ci = types::initializeCellIndicator(m.getDimensions());

        // Find the rooms in the maze and process.
        RoomFinder roomFinder(m);
        const auto &cellsToRooms = roomFinder.getCellToRoom();
        const auto &roomContents = roomFinder.getRoomContents();
        for (const auto &r: roomContents) {
            const auto &[idx, contents] = r;
#ifdef DEBUG
            cout << "Processing room " << idx << ':' << endl;
#endif
            processRoom(m, contents);

            // Mark the room cells as visited.
            for (const auto &c: contents) {
                const auto &[cx, cy] = c;
                ci[cx][cy] = true;
            }
        }

        /*** PROCESSING ***/
        // Get a list of the dead ends. We start by covering dead ends and all that is left after
        // that are isolated cells and loops.
        auto deadEnds = m.findDeadEnds();

        // Create the queue, which comprises a starting vertex, and a list of the covered cells.
        struct EdgeStart {
            const WeightedGraphVertex u{};
            types::CellCollection cells{};
        };
        std::queue<EdgeStart> edgeQueue;

#ifdef DEBUG
        cout << "Dead ends: " << typeclasses::Show<types::CellCollection>::show(deadEnds) << endl;
#endif
        while (!deadEnds.empty()) {
            // If the queue is empty, make a new start at a dead end for it.
            if (edgeQueue.empty()) {
                // Keep popping dead ends until we find an unvisited one.
                while (!deadEnds.empty()) {
                    const auto &deadEnd = deadEnds.back();
                    const auto &[dx, dy] = deadEnd;
#ifdef DEBUG
                    cout << "Considering the dead end: " << typeclasses::Show<types::Cell>::show(deadEnd) << endl;
#endif
                    if (ci[dx][dy]) {
#ifdef DEBUG
                        cout << "\tAlready covered... skipping." << endl;
#endif
                        deadEnds.pop_back();
                    } else
                        break;
                }

                // If we are out of dead ends, then we are done.
                if (deadEnds.empty())
                    break;

                const auto deadEnd = deadEnds.back();
                const auto &[dx, dy] = deadEnd;
                deadEnds.pop_back();
#ifdef DEBUG
                cout << "\tUsing dead end: " << typeclasses::Show<types::Cell>::show(deadEnd) << endl;
#endif

                const WeightedGraphVertex u = boost::add_vertex(graph);
                vertexCell[deadEnd] = u;
                edgeQueue.emplace(EdgeStart{u, types::CellCollection{deadEnd}});

                // Mark the cell as visited.
                ci[dx][dy] = true;
            }

            EdgeStart edgeStart = edgeQueue.front();
            edgeQueue.pop();

            // Mark the current cell as visited.
            const auto &curCell = edgeStart.cells.back();
            const auto &[curX, curY] = curCell;
            ci[curX][curY] = true;

            // Get the neighbours of the last cell in the queue, and remove all vertices that
            // have already been visited to get the unvisited neighbours.
            const auto nbrs = m.neighbours(edgeStart.cells.back());
            types::CellCollection unvisitedNbrs;
            std::copy_if(nbrs.cbegin(), nbrs.cend(), std::back_inserter(unvisitedNbrs), [&ci](const types::Cell &c) {
               const auto [cx, cy] = c;
               return !ci[cx][cy];
            });
//            for (const types::Cell &c: nbrs) {
//                const auto[cx, cy] = c;
//                if (!(ci[cx][cy]))
//                    unvisitedNbrs.emplace_back(c);
//            }

            // We act depending on the number of neighbours and the possibility that we entered a room.
            // 1. If we are on a room cell, we have entered a room.
            // 2. If we have 1 neighbour and 0 unvisited neighbours, we are at the end of a passage.
            // 3. If we have 1 neighbour and 1 unvisited neighbours, we are at the start of a passage.
            //    If we have 2 neighbours and 1 unvisited neighbour, we are in the middle of a passage.
            // 4. If we have 2 neighbours and 0 univisted neighbours, we are in a loop or in a passage already explored
            //    or being explored from the other direction..
            // 5. If we have 3 neighbours, we are at a T intersection.
            //    If we have 4 neighbours, we are at a + intersection.
            // In all cases but 3 and 4, we want to create an edge.
            // NOTE that we can get into some bizarre situations where the same EdgeStart seed ends up queued
            //      up twice. This could happen, for example, if we have an edge (a,c) and (b,c) where c is a
            //      junction node, and a and b both create these edges before the EdgeStart with c is run.

            // Thus, to avoid these edge cases, we always check to make sure an edge is not already in our graph
            //      before trying to add one. This tries to add an edge between edgeStart.u and v for the given rule.
            const auto add_edge = [&](const int rule, const auto &v) {
                // Calculate the weight of this edge.
                const auto weight = edgeStart.cells.size() - 1;

#ifdef DEBUG
                cout << rule << ": ";
#endif
                const auto [e0, exists] = boost::edge(edgeStart.u, v, graph);

                // This is rather complex because it is possible that we have two different unbroken paths between two
                // vertices. Example:
                //
                // --------
                // |      |
                // u------v
                //
                // We only record the edge of minimum weight as we are only interested in the shortest paths between
                // vertices.
                if (exists) {
                    const auto weight0 = *(reinterpret_cast<int*>(e0.m_eproperty));
#ifdef DEBUG
                    cout << "Edge " << e0 << " already exists with weight: " << weight0 << ", new weight: " << weight << endl;
#endif
                    if (weight <= weight0)
                        return;
                    else {
#ifdef DEBUG
                        cout << "\tRemoving old edge..." << endl;
#endif
                        boost::remove_edge(e0, graph);
                    }
                }

                const auto [e, success] = boost::add_edge(edgeStart.u, v, edgeStart.cells.size() - 1, graph);
                if (!success) {
#ifdef DEBUG
                    cout << "FAILURE: could not find or add edge " << e << " over cells:" << endl;
#endif
                } else {
                    edges[e] = edgeStart.cells;
#ifdef DEBUG
                    cout << "Added edge " << e << " with weight " << *(reinterpret_cast<int*>(e.m_eproperty))
                         << " and cells:" << endl;
#endif
                }
#ifdef DEBUG
                cout << '\t' << typeclasses::Show<types::CellCollection>::show(edgeStart.cells) << endl;
#endif
            };

            // Lambda to add a vertex if we are processing a queue step.
            const auto add_vertex = [&]() {
                const auto v = boost::add_vertex(graph);
                vertexCell[curCell] = v;
                return v;
            };

            /*** 1 ***/
            if (cellsToRooms[curX][curY] != -1) {
                // We entered a room: close off.
                const auto &v = vertexCell[curCell];
                add_edge(1, v);
            }

            /*** 2 ***/
            else if (nbrs.size() == 1 && unvisitedNbrs.empty()) {
                // End of a passage: add a final vertex and close off.
                add_edge(3, add_vertex());
            }

            /*** 3 ***/
            else if ((nbrs.size() == 1 || nbrs.size() == 2 ) && unvisitedNbrs.size() == 1) {
                // This covers two cases:
                // 1. nbrs = 1: We are at the very start of a passage.
                // 2. nbrs = 2: We are in the middle of a passage.
                // In either case, extend with the unvisited cell and enqueue if we haven't already done so.
                const auto &[ux, uy] = unvisitedNbrs[0];
                if (!ci[ux][uy]) {
                    edgeStart.cells.emplace_back(unvisitedNbrs[0]);
                    edgeQueue.emplace(edgeStart);
                }
            }

            /*** 4 ***/
            else if (nbrs.size() == 2 && unvisitedNbrs.empty()) {
                // We are in a loop or in a short passage of length 2 existing between two cells.
                // There is one cell behind and one cell ahead. Determine which is ahead.
                // The current cell is the last cell in the list edgeStart.cells, so the behind cell is the cell
                // before that.
                assert(edgeStart.cells.size() >= 2);
                const types::Cell &behindCell = edgeStart.cells[edgeStart.cells.size() - 2];
                const types::Cell &aheadCell = (behindCell == nbrs[0]) ? nbrs[1] : nbrs[0];

                // Keep going: the algorithm will sort out duplicates when we reach an edge point.
                edgeStart.cells.emplace_back(aheadCell);
                edgeQueue.emplace(edgeStart);
            }

            /*** ERROR CASE: this should never happen ***/
            else if (nbrs.size() == unvisitedNbrs.size()) {
                throw std::domain_error("Unexpected maze configuration.");
            }

            /*** 5 ***/
            else {
                // We are at a junction. Check to see if a vertex exists for this cell, and if not, create one.
                const auto iter = vertexCell.find(curCell);
                const auto v = (iter == vertexCell.end()) ? add_vertex() : iter->second;

                // Add an edge.
                add_edge(6, v);

                // Create new EdgeStarts for each unvisited neighbour and enqueue them.
                // Note that we could theoretically enqueue a case already enqueued, hence the safeguard of
                // checking that edges do not exist when adding them.
                for (const auto &n: unvisitedNbrs)
                    edgeQueue.emplace(EdgeStart{v, types::CellCollection{curCell, n}});
            }
        }

        // Now we have taken care of all the dead ends: all that is left are isolated cells and loops.
        // Iterate over the remaining cells.
        for (auto y = 0; y < height; ++y) {
            for (auto x = 0; x < width; ++x) {
                // Skip visited cells.
                if (ci[x][y]) continue;

                // For cells out of bounds (e.g. walls in a ThickMaze) or cells in a room,
                // mark visited and skip.
                if (cellsToRooms[x][y] != -1 || !m.cellInBounds(x, y)) {
                    ci[x][y] = true;
                    continue;
                }

                // Perform a BFS starting here.
                const auto bfsResults = m.performBFSFrom(types::cell(x, y));
                const auto &loop = bfsResults.connectedCells;

                // Create a vertex and loop in the graph with these cells.
                const auto v = boost::add_vertex(graph);
                vertexCell[types::cell(x, y)] = v;

                // Mark all cells in the loop as visited.
                for (const auto &l: loop) {
                    const auto &[lx, ly] = l;
                    ci[lx][ly] = true;
                }

#ifdef DEBUG
                cout << "Loop of size " << loop.size() << ": " << typeclasses::Show<types::CellCollection>::show(loop) << endl;
#endif
                const auto [e, success] = boost::add_edge(v, v, loop.size() - 1, graph);

                // Mark all the cells as visited and add a disconnected vertex in a loop to the graph.
                edges[e] = loop;
            }
        }
    }


    template<typename T>
    void SquashedMaze::processRoom(const types::AbstractMaze<T> &m, const types::CellCollection &cc) {
        // We begin by finding all of the entrances to the room.
        // Entrances are cells that have neighbours in the maze outside of the cells comprising the room.
#ifdef DEBUG
        cout << "\tCells: " << typeclasses::Show<types::CellCollection>::show(cc) << endl;
#endif
        types::CellCollection entrances;
        for (const auto &c: cc) {
            auto nbrs = m.neighbours(c);

            // Remove all the cells in the room to leave only the cells outside of the room.
            for (const auto &cother: cc) {
                const auto iter = std::find(nbrs.begin(), nbrs.end(), cother);
                if (iter != nbrs.end())
                    nbrs.erase(iter);
            }

            // If there are any neighbours left, we are an entrance.
            if (!nbrs.empty())
                entrances.emplace_back(c);
        }

#ifdef DEBUG
        cout << "\tEntrances: " << typeclasses::Show<types::CellCollection>::show(entrances) << endl;
#endif

        // Now we have our entrances: create a vertex for each.
        for (const auto &c: entrances) {
            vertexCell[c] = boost::add_vertex(graph);
#ifdef DEBUG
            cout << "\tAssigning " << vertexCell[c] << " to " << typeclasses::Show<types::Cell>::show(c) << endl;
#endif
        }

        // Now find the shortest path between every pair of vertices, storing the distance and the path.
        // BfsData is the data we want to accumulate during the BFS.
        struct BfsData {
            int distance = -1;
            types::CellCollection path{};
        };

        for (auto i = 0; i < entrances.size(); ++i) {
            const auto &u = entrances[i];
            for (auto j = i + 1; j < entrances.size(); ++j) {
                const auto &v = entrances[j];

                // Initialize a map of BfsData for each cell in the room, marking them unvisited.
                std::map<types::Cell, BfsData> bfsData;
                for (const auto &c: cc) {
                    bfsData[c] = BfsData{};
                }

                // Now we create the BFS queue, and initialize it with u.
                std::queue<types::Cell> bfsQueue;
                bfsQueue.emplace(u);
                bfsData[u] = BfsData{0, types::CellCollection{u}};

                while (!bfsQueue.empty()) {
                    // Get the next cell to process.
                    const auto c = bfsQueue.front();
                    bfsQueue.pop();

                    // Get the BfsData for the current cell.
                    const auto &cBfsData = bfsData[c];

                    // Get the unvisited neighbours in the room.
                    const auto nbrs = m.neighbours(c);
                    for (const auto &n: nbrs) {
                        // if we can't find the neighbour, it is outside of the room, so skip.
                        const auto iter = bfsData.find(n);
                        if (iter == bfsData.end())
                            continue;

                        // If the cell has already been visited (i.e. has populated BfsData), skip.
                        const auto &[dist, cells] = iter->second;
                        if (dist != -1) continue;

                        // It has not been visited. Fill out its BfsData.
                        auto pathn = cBfsData.path;
                        pathn.emplace_back(n);
                        bfsData[n] = BfsData{cBfsData.distance + 1, pathn};

                        // Add it to the queue.
                        bfsQueue.push(n);
                    }

                    // If we have found v, stop.
                    const auto &[vdist, vpath] = bfsData[v];
                    if (vdist != -1)
                        break;
                }

                // Make sure we have found v.
                const auto &[vdist, vpath] = bfsData[v];
                assert(vdist != -1);

                // Insert the weighted edge and modify edges to record the path.
                const auto [e, success] = boost::add_edge(vertexCell[u], vertexCell[v], vdist, graph);
                edges[e] = vpath;
#ifdef DEBUG
                cout << "\tEdge " << e << " of weight " << vdist << endl;
#endif
            }
        }
    }
}

