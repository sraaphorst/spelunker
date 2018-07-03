/**
 * SquashedMaze.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * A squashed maze is a representation of a maze as a graph that maintains many of its fundamental characteristics.
 * The goal is to create a basic graph that preserves distance between the "important" vertices, i.e. the only
 * vertices where distance could be maximized or involved in the choice of path.
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
     * Edges are weighted by the number of cells they cover. If more than one path exists between two vertices,
     * the one with the lower weight is selected.
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
         * @return the list of entrance cells for the room
         */
        template<typename T>
        types::CellCollection processRoom(const types::AbstractMaze<T> &m, const types::CellCollection &cc);

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
        // A convenience method to extract the weight of a weighted edge.
        const auto wt = [](const auto &e) {
            return *(reinterpret_cast<int *>(e.m_eproperty));
        };

        /*** PREPROCESSING ***/
        // Make sure that every cell is covered.
        const auto width = m.getWidth();
        const auto height = m.getHeight();
        auto ci = types::initializeCellIndicator(m.getDimensions());

        // Begin by marking off all of the unreachable cells as visited so we no longer have to worry about them.
        for (auto y = 0; y < height; ++y)
            for (auto x = 0; x < width; ++x)
                ci[x][y] = !m.cellInBounds(x, y);

        // Begin by creating a vertex for each pertinent cell in the maze. These comprise:
        // 1. The entrances to rooms.
        // 2. The dead ends.
        // 3. T or + junctures.

        /*** QUEUE ***/
        // u is the start vertex.
        // cells represents the cells visited thus far.
        struct EdgeStart {
            const WeightedGraphVertex u{};
            types::CellCollection cells{};
        };
        std::queue<EdgeStart> edgeQueue;

        /*** ROOM ENTRANCES ***/
        // Find the rooms in the maze and process.
        // Keep track of all the room cells, minus the entrances: we don't care about these.
        types::CellCollection roomCells;

        RoomFinder roomFinder(m);
        const auto &cellsToRooms = roomFinder.getCellToRoom();
        const auto &roomContents = roomFinder.getRoomContents();
        for (const auto &r: roomContents) {
            const auto &[idx, contents] = r;
            const auto entrances = processRoom(m, contents);
#ifdef DEBUG
            cout << "Processed room " << idx << ':' << endl;
            cout << "\tContents: " << typeclasses::Show<types::CellCollection>::show(contents) << endl;
            for (const auto &e: entrances)
                cout << "\tEntrance " << typeclasses::Show<types::Cell>::show(e)
                     << " assigned to vertex " << vertexCell[e] << endl;
            cout << endl;
#endif

            // Mark ALL the room cells as visited.
            for (const auto &c: contents) {
                // If not an entrance cell, emplace.
                if (vertexCell.find(c) == vertexCell.end())
                    roomCells.emplace_back(c);
                const auto &[cx, cy] = c;
                ci[cx][cy] = true;
            }

            // Now create and enqueue a node for each entrance.
            for (const auto &e: entrances)
                edgeQueue.emplace(EdgeStart{vertexCell[e], types::CellCollection{e}});
        }

        /*** DEAD ENDS ***/
        // Get a list of the dead ends. We start by covering dead ends and all that is left after
        // that are isolated cells and loops.
        auto deadEnds = m.findDeadEnds();
        for (const auto &de: deadEnds) {
            // Create a vertex for the dead end.
            const auto v = boost::add_vertex(graph);
            vertexCell[de] = v;

            // Mark the dead end as visited.
            const auto &[dx, dy] = de;
            ci[dx][dy] = true;

            // Create and enqueue a node for each dead end.
            edgeQueue.emplace(EdgeStart{v, types::CellCollection{de}});
        }

#ifdef DEBUG
        cout << "Dead ends: " << typeclasses::Show<types::CellCollection>::show(deadEnds) << endl;
        for (const auto &de: deadEnds)
            cout << "\tDead end " << typeclasses::Show<types::Cell>::show(de) << " assigned to vertex "
                 << vertexCell[de] << endl;
        cout << endl;
#endif

        /*** JUNCTIONS ***/
#ifdef DEBUG
        cout << "Analyzing junctions..." << endl;
#endif
        // Iterate and find all the junctions. Create a vertex for each and place in the maze.
        for (auto y = 0; y < height; ++y) {
            for (auto x = 0; x < width; ++x) {
                // If this cell is already covered, it is either a dead end or part of a room: skip it.
                if (ci[x][y]) continue;

                // A cell is a junction if it only has 0 walls (+) or 1 wall (T).
                const auto c = types::cell(x, y);
                if (m.numCellWalls(c) > 1) continue;

                // Create a vertex for the junction and mark it visited.
                const auto v = boost::add_vertex(graph);
                vertexCell[c] = v;
                ci[x][y] = true;
#ifdef DEBUG
                cout << "\tJunction " << typeclasses::Show<types::Cell>::show(c) << " assigned to vertex "
                     << vertexCell[c] << endl;
#endif
                // Create and eqneue a node.
                edgeQueue.emplace(EdgeStart{v, types::CellCollection{c}});
            }
        }
#ifdef DEBUG
        cout << endl;
#endif

        // Basic idea: get all neighbours.
        // This is a bit more complicated: ignore neighbours that are non-entrance room cells since we don't care
        //    about them.
        // For unvisited neighbours, extend and enqueue.
        // For visited neighbours, if they have a vertex, try to create an edge.
        //    If already edge, remove and replace if our weight is smaller.
        //    If they do not have an edge, extend and enqueue.

        // Create a couple simple lambdas to avoid clutter.
        const auto cellIn = [](const types::Cell &c, const types::CellCollection &cc) {
            return std::find(cc.cbegin(), cc.cend(), c) != cc.cend();
        };
        const auto cellNotIn = [&cellIn](const types::Cell &c, const types::CellCollection &cc) {
            return !cellIn(c, cc);
        };

        while (!edgeQueue.empty()) {
            const auto edgeStart = edgeQueue.front();
            edgeQueue.pop();
#ifdef ADEBUG
            cout << "Processing node starting at u: " << edgeStart.u
                 << ", cells: " << typeclasses::Show<types::CellCollection>::show(edgeStart.cells) << endl;
#endif
            // Get all of the neighbours not in the cell list, and divide into visited and unvisited.
            const auto &cell = edgeStart.cells.back();
            const auto &[cellx, celly] = cell;
            ci[cellx][celly] = true;
            const auto neighbours = m.neighbours(cell);

            types::CellCollection roomNeighbours;
            std::copy_if(neighbours.cbegin(), neighbours.cend(), std::back_inserter(roomNeighbours),
                         [&edgeStart, &cellIn, &roomCells](const types::Cell &c) {
                             return cellIn(c, roomCells);
                         });

            types::CellCollection inEdgeStartNeighbours;
            std::copy_if(neighbours.cbegin(), neighbours.cend(), std::back_inserter(inEdgeStartNeighbours),
                         [&edgeStart, &cellIn, &cellNotIn, &roomCells](const types::Cell &c) {
                             return cellIn(c, edgeStart.cells) && cellNotIn(c, roomCells);
                         });

            types::CellCollection visitedNeighbours;
            std::copy_if(neighbours.cbegin(), neighbours.cend(), std::back_inserter(visitedNeighbours),
                         [&ci, &edgeStart, &cellNotIn, &roomCells](const types::Cell &c) {
                             const auto[cx, cy] = c;
                             return ci[cx][cy] && cellNotIn(c, edgeStart.cells) && cellNotIn(c, roomCells);
                         });

            types::CellCollection unvisitedNeighbours;
            std::copy_if(neighbours.cbegin(), neighbours.cend(), std::back_inserter(unvisitedNeighbours),
                         [&ci, &edgeStart, &cellNotIn, &roomCells](const types::Cell &c) {
                             const auto[cx, cy] = c;
                             return !ci[cx][cy] && cellNotIn(c, edgeStart.cells) && cellNotIn(c, roomCells);
                         });

            assert(neighbours.size() == roomNeighbours.size() + inEdgeStartNeighbours.size() + visitedNeighbours.size() + unvisitedNeighbours.size());

#ifdef ADEBUG
            cout << "Neighbours: " << typeclasses::Show<types::CellCollection>::show(neighbours) << endl;
            cout << "In edge already: " << typeclasses::Show<types::CellCollection>::show(inEdgeStartNeighbours) << endl;
            cout << "Visited neighbours: " << typeclasses::Show<types::CellCollection>::show(visitedNeighbours) << endl;
#endif
            // For visited neighbours, if they have a vertex, attempt to create an edge.
            // Otherwise, ignore.
            for (const auto &vn: visitedNeighbours) {
#ifdef ADEBUG
                cout << "\tConsidering neighbour " << typeclasses::Show<types::Cell>::show(vn) << ": ";
#endif
                // Extract second as first will be vn.
                const auto iter = vertexCell.find(vn);

                if (iter == vertexCell.end()) {
#ifdef ADEBUG
                    cout << "Not a vertex cell. Extending path." << endl;
#endif
                    auto newcells = edgeStart.cells;
                    newcells.emplace_back(vn);
                    edgeQueue.emplace(EdgeStart{edgeStart.u, newcells});
                    continue;
                }
                const auto v = iter->second;
#ifdef ADEBUG
                cout << "vertex is " << v << endl;
#endif
                // Check if there is already an edge, and replace it if this one is shorter.
                const auto weight = edgeStart.cells.size();
                auto[e0, exists] = boost::edge(edgeStart.u, v, graph);
#ifdef ADEBUG
                if (exists)
                    cout << "\t\tEdge " << e0 << " exists with weight " << wt(e0) << ", new weight is " << weight
                         << endl;
#endif
                if (exists && weight < wt(e0)) {
#ifdef ADEBUG
                    cout << "\t\tRemoving old edge." << endl;
#endif
                    boost::remove_edge(edgeStart.u, v, graph);
                    exists = false;
                }
                if (!exists) {
                    const auto[e, success] = boost::add_edge(edgeStart.u, v, weight, graph);
                    edges[e] = edgeStart.cells;
#ifdef DEBUG
                    cout << "\t\tAdding edge " << e << " with weight " << weight << endl;
#endif
                }
            }

#ifdef ADEBUG
            cout << "Unvisited neighbours: " << typeclasses::Show<types::CellCollection>::show(visitedNeighbours) << endl
                 << endl;
#endif
            // For unvisited neighbours, extend and enqueue.
            for (const auto &un: unvisitedNeighbours) {
                auto newcells = edgeStart.cells;
                newcells.emplace_back(un);
                edgeQueue.emplace(EdgeStart{edgeStart.u, newcells});
            }
        }

        /*** LEFTOVER LOOPS ***/
        // Now we have taken care of all the dead ends: all that is left are isolated cells and loops.
        // Iterate over the remaining cells.
        for (auto y = 0; y < height; ++y) {
            for (auto x = 0; x < width; ++x) {
                // Skip visited cells.
                if (ci[x][y]) continue;

                // Perform a BFS starting here.
                const auto cell = types::cell(x, y);
                const auto bfsResults = m.performBFSFrom(cell);
                const auto &loop = bfsResults.connectedCells;

                // Create a vertex and loop in the graph with these cells.
                const auto v = boost::add_vertex(graph);
                vertexCell[cell] = v;

                // Mark all cells in the loop as visited.
                for (const auto &l: loop) {
                    const auto &[lx, ly] = l;
                    ci[lx][ly] = true;
                }

                // Add the edge for the loop.
                const auto[e, success] = boost::add_edge(v, v, loop.size() - 1, graph);
                edges[e] = loop;
#ifdef DEBUG
                cout << "\t\tAdding loop " << e << " with weight " << loop.size() - 1 << endl;
#endif
            }
        }
    }


    template<typename T>
    types::CellCollection SquashedMaze::processRoom(const types::AbstractMaze<T> &m, const types::CellCollection &cc) {
        // We begin by finding all of the entrances to the room.
        // Entrances are cells that have neighbours in the maze outside of the cells comprising the room.
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

        // Now we have our entrances: create a vertex for each.
        for (const auto &c: entrances) {
            vertexCell[c] = boost::add_vertex(graph);
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
                const auto[e, success] = boost::add_edge(vertexCell[u], vertexCell[v], vdist, graph);
                edges[e] = vpath;
            }
        }

        return entrances;
    }
}

