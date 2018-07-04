/**
 * SquashedMaze.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

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
#include "SquashedMaze.h"

#ifdef DEBUG
#include <iostream>
using namespace std;
#endif

namespace spelunker::squashedmaze {

    SquashedMaze::SquashedMaze(const spelunker::types::AbstractMaze &m) {
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
        // These are cells that are unnecessary to visit, as no path can be made shorter by doing so,
        // so we avoid them entirely.
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


        // Code common for dead ends and junctions:
        // Create a vertex for the cell, and then enqueue an EdgeStart for the cell to represent
        // creating an edge beginning at this vertex in the squashed graph.
        const auto collectionToVertices = [&](const types::CellCollection &cc) {
            for (const auto &c: cc) {
                // Create a vertex for the cell.
                const auto v = boost::add_vertex(graph);
                vertexCell[c] = v;

                // Mark the cell as visited.
                const auto &[cx, cy] = c;
                ci[cx][cy] = true;

                // Enqueue an edge start.
                edgeQueue.emplace(EdgeStart{v, types::CellCollection{c}});
            }
        };

#ifdef DEBUG
        // Print information about the collection and what cells were turned into what vertices.
        const auto debugCollectionInformation = [&](const std::string name, const types::CellCollection &cc) {
            cout << name << " list: " << typeclasses::Show<types::CellCollection>::show(cc) << endl;
            for (const auto &c: cc)
                cout << '\t' << name << ' ' << typeclasses::Show<types::Cell>::show(c) << " assigned to vertex "
                     << vertexCell[c] << endl;
            cout << endl;
        };
#endif


        /*** DEAD ENDS ***/
        // Get a list of the dead ends, create a vertex for each, and enqueue an edge start.
        auto deadEnds = m.findDeadEnds();
        collectionToVertices(deadEnds);
#ifdef DEBUG
        debugCollectionInformation("Dead end", deadEnds);
#endif

        /*** JUNCTIONS ***/
        // Get a list of the junctions in the maze. We create a vertex for each, and enqueue an edge start.
        auto junctions = m.findJunctions();
        collectionToVertices(junctions);
#ifdef DEBUG
        debugCollectionInformation("Junction", junctions);
#endif

        /**
         * Basic idea of the queue:
         * 1. If the queue is empty, complete.
         * 2. Pick a cell from the queue beginning as vertex u and find its:
         *    (a) visited neighbours, minus any non-entrance room cells and any cells visited already by this EdgeStart.
         *    (b) unvisited neighbours, minus the same cells as in (a).
         * 3. For a visited neighbour, check if the cell has a vertex associated with it, say v.
         *    (a) If it does, then we consider the edge to be done and have weight equal to the number of moves it took
         *        to get from the cell represented by vertex u to the cell represented by vertex v.
         *        If there is no edge {u,v} or there is an edge {u,v} of higher weight, add / replace with this {u,v}.
         *    (b) If it has no vertex associated with it, then extend by this cell and re-enqueue.
         * 4. For an unvisited neighbour, extend by this cell and enqueue.
         */

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

            // Get all of the neighbours not in the previously visited cell list or the room cell list,
            // and divide into visited and unvisited.
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


            // For visited neighbours, if they have a vertex, attempt to create an edge.
            // Otherwise, extend and enqueue.
            for (const auto &vn: visitedNeighbours) {
                const auto iter = vertexCell.find(vn);

                // If we cannot find a vertex for this cell, extend and enqueue.
                if (iter == vertexCell.end()) {
                    auto newcells = edgeStart.cells;
                    newcells.emplace_back(vn);
                    edgeQueue.emplace(EdgeStart{edgeStart.u, newcells});
                    continue;
                }

                // Check if there is already an edge, and replace it if this one is shorter.
                const auto v = iter->second;
                const auto weight = edgeStart.cells.size();
                auto[e0, exists] = boost::edge(edgeStart.u, v, graph);

                if (exists && weight < wt(e0)) {
                    boost::remove_edge(edgeStart.u, v, graph);
                    exists = false;
                }

                // If no edge exists at this point, add {u,v} with the weight.
                if (!exists) {
                    const auto[e, success] = boost::add_edge(edgeStart.u, v, weight, graph);
                    edges[e] = edgeStart.cells;
#ifdef DEBUG
                    cout << "Adding edge " << e << " with weight " << weight << endl;
#endif
                }
            }

            // For unvisited neighbours, extend and enqueue.
            for (const auto &un: unvisitedNeighbours) {
                auto newcells = edgeStart.cells;
                newcells.emplace_back(un);
                edgeQueue.emplace(EdgeStart{edgeStart.u, newcells});
            }
        }

        /*** LEFTOVER ISOLATED CELLS / LOOPS ***/
        // Now we have taken care of all the dead ends, rooms, and junctions.
        // All that is left are isolated cells and loops. Iterate over the cells and pick these out.
        for (auto y = 0; y < height; ++y) {
            for (auto x = 0; x < width; ++x) {
                // Skip visited cells.
                if (ci[x][y]) continue;

                // We have an unvisited cell to process.
                // Perform a simple BFS starting here.
                const auto cell = types::cell(x, y);
                const auto bfsResults = m.performBFSFrom(cell);
                const auto &loop = bfsResults.connectedCells;

                // Create a vertex for the cell, and add the edge for the loop.
                const auto v = boost::add_vertex(graph);
                vertexCell[cell] = v;
                const auto[e, success] = boost::add_edge(v, v, loop.size() - 1, graph);
                edges[e] = loop;

                // Mark all cells in the loop as visited.
                for (const auto &l: loop) {
                    const auto &[lx, ly] = l;
                    ci[lx][ly] = true;
                }
#ifdef DEBUG
                cout << "Adding loop " << e << " with weight " << loop.size() - 1 << endl;
#endif
            }
        }
    }


    types::CellCollection SquashedMaze::processRoom(const types::AbstractMaze &m, const types::CellCollection &cc) {
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
                        const auto dist = iter->second.distance;
                        const auto &cells = iter->second.path;
                        if (dist != -1) continue;

                        // It has not been visited. Fill out its BfsData.
                        auto pathn = cBfsData.path;
                        pathn.emplace_back(n);
                        bfsData[n] = BfsData{cBfsData.distance + 1, pathn};

                        // Add it to the queue.
                        bfsQueue.push(n);
                    }

                    // If we have found v, stop.
                    const auto vdist = bfsData[v].distance;
                    const auto &vpath = bfsData[v].path;
                    if (vdist != -1)
                        break;
                }

                // Make sure we have found v.
                const auto vdist = bfsData[v].distance;
                const auto &vpath = bfsData[v].path;
                assert(vdist != -1);

                // Insert the weighted edge and modify edges to record the path.
                const auto[e, success] = boost::add_edge(vertexCell[u], vertexCell[v], vdist, graph);
                edges[e] = vpath;
            }
        }

        return entrances;
    }
}