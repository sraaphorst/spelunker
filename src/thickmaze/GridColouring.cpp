/**
 * GridColouring.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <algorithm>
#include <functional>
#include <map>
#include <numeric>
#include <queue>
#include <stdexcept>
#include <vector>

#ifdef DEBUG
#include <iostream>
using namespace std;
#endif

#include "math/Partition.h"
#include "GridColouring.h"

GridColouring::GridColouring(int p_ux, int p_vx, int p_vy)
        : ux(p_ux), vx(p_vx), vy(p_vy), numColours(p_ux * p_vy),
          rows(p_ux * p_vy / std::gcd(p_vx, p_vy)) {

    // Initialize the reverse lookup table to all -1.
    reverseLookup = ReverseLookup(numColours, std::make_pair(-1, -1));

    // Initialize the lookup table with -1.
    cellColours = ColourLookup(ux, ColourLookupRow(rows, -1));

    // Assign colours to the cells and store the reverse lookup.
    Colour c = 0;
    for (auto y = 0; y < vy; ++y) {
        for (auto x = 0; x < ux; ++x) {
            for (auto z = 0; z < ux; ++z) {
                int idx_x = mod(x + z * vx, ux);
                int idx_y = mod(y + z * vy, rows);
                cellColours[idx_x][idx_y] = c;
                if (reverseLookup[c].first == -1) {
                    reverseLookup[c] = std::make_pair(idx_x, idx_y);
                }
            }
            ++c;
        }
    }

#ifdef DEBUG
    cout << "Lookup table:" << endl;
    for (auto y = 0; y < rows; ++y) {
        for (auto x = 0; x < ux; ++x) {
            cout << cellColours[x][y] << ' ';
        }
        cout << std::endl;
    }
#endif
}

// Convenience function to avoid lots of lambdas.
template<template<typename, typename> class Container,
        typename Value,
        typename Allocator=std::allocator<Value> >
static bool contains(const Container<Value, Allocator> &vec, const Value elem) {
    return std::find(vec.begin(), vec.end(), elem) != vec.end();
}

// Convenience function to avoid mucking around with iterators and sorting.
template<template<typename, typename> class Container,
        typename Value,
        typename Allocator=std::allocator<Value> >
static bool intersects(const Container<Value, Allocator> &vec1, const Container<Value, Allocator> &vec2) {
    for (auto v1: vec1)
        if (std::find(vec2.begin(), vec2.end(), v1) != vec2.end())
            return true;
    return false;
};

GridColouring::AbortPrematurely GridColouring::neverStop =
        [](const GridColouring::CandidateConfiguration &c) { return false; };
GridColouring::AbortPrematurely GridColouring::stopAfterRoom0 =
        [](const GridColouring::CandidateConfiguration &c) { return c.roomColour != 0; };

std::vector<GridColouring::CandidateConfiguration> GridColouring::wallCandidates(int maxSize,
                                                                                 GridColouring::AbortPrematurely &aborter) const {
    // Brute-force all possibilities, of which there are many.
    std::vector<CandidateConfiguration> configurations;

    // Iterate over the candidates for room colour.
    bool aborted = false;
    for (auto room = 0; room < numColours; ++room) {
        auto roomNbrs = neighbourhood(ColourCollection{room});

#ifdef DEBUG
        cout << endl;
        cout << "***** ROOM " << room << " *****" << endl;
        cout << "Neighbours:";
        for (auto n: roomNbrs)
            cout << ' ' << n;
        cout << endl;
#endif

        // Iterate over all partitions.
        try {
            for (spelunker::math::partition::iterator iter(numColours - 1); ; ++iter) {
                // Stop if we've reached the maximum size.
                if (maxSize != -1 && configurations.size() >= maxSize) {
                    aborted = true;
                    break;
                }

                // If we only have pillars, skip.
                if (iter.subsets() == 1)
                    continue;

                // We need to figure out what, amongst the partition, are walls, and what are pillars.
                // Begin by assuming that everything is a wall.
                auto ptn = *iter;

                // Create the candidates for the walls as lists of colours.
                // Remember to skip over the room colour by incrementing everything greater than or equal to
                // it by 1.
                std::vector<ColourCollection> wallCandidates;
                wallCandidates.resize(iter.subsets());
                for (auto i=0; i < ptn.size(); ++i)
                    wallCandidates[ptn[i]].emplace_back(i < room ? i : i + 1);

#ifdef DEBUG
                cout << endl << "Trying new partition: ";
                for (auto w: wallCandidates) {
                    cout << '[';
                    for (auto i=0; i < w.size(); ++i) {
                        cout << w[i];
                        if (i < w.size() - 1)
                            cout << ',';
                    }
                    cout << "] ";
                }
                cout << endl;
#endif

#ifdef DEBUG
                cout << "Wall candidates:" << endl;
                for (auto w: wallCandidates) {
                    cout << '\t';
                    for (auto i: w)
                        cout << i << ' ';
                    cout << endl;
                }
#endif

                // If a wall candidate is not contiguous or not adjacent to the room colour, drop it.
                // Based on rooms requiring four walls, the remaining should all be walls or the partition
                // is invalid.
                std::vector<ColourCollection> walls;
                walls.reserve(wallCandidates.size());
                std::copy_if(wallCandidates.begin(), wallCandidates.end(),
                             std::back_inserter(walls),
                             [this, &roomNbrs](auto c) { return isContiguous(c) && intersects(roomNbrs, c); }
                );

#ifdef DEBUG
                cout << "Contiguous wall candidates:" << endl;
                for (auto w: walls) {
                    cout << '\t';
                    for (auto i: w)
                        cout << i << ' ';
                    cout << endl;
                }
#endif
                // 1. Every room must be adjacent to four walls.
                auto roomAdjacency = 0;
                for (auto n: roomNbrs)
                    for (auto w: walls)
                        if (contains(w, n))
                            ++roomAdjacency;

#ifdef DEBUG
                cout << "Room adjacency = " << roomAdjacency << endl;
#endif
                if (roomAdjacency != 4)
                    continue;

                // 2. Every wall must connect exactly two rooms.
                // Find their neighbours.
                std::vector<Neighbourhood> wallNbrs;
                wallNbrs.resize(walls.size());
                for (auto i = 0; i < walls.size(); ++i)
                    wallNbrs[i] = neighbourhood(walls[i]);

                auto candidate = true;
                for (const auto w: wallNbrs) {
                    auto adjacencyCount = std::count(w.begin(), w.end(), room);
#ifdef DEBUG
                    cout << "Wall ";
                    for (auto cell: w)
                        cout << cell << ' ';
                    cout << " has adjacency count: " << adjacencyCount << endl;
#endif
                    if (adjacencyCount != 2) {
                        candidate = false;
                        break;
                    }
                }
                if (!candidate) continue;

                // 3. A wall is adjacent to a room iff that room is adjacent to the wall.
                for (auto i = 0; i < walls.size(); ++i) {
                    // 3a. If room is a wall's neighbour, then the wall must have a cell in the room's neighbours.
                    if (contains(wallNbrs[i], room) && !intersects(roomNbrs, walls[i])) {
                        candidate = false;
                        break;
                    }
                    // 3b. If a wall has a cell in the room's neighbours, then the room must be that wall's neighbour.
                    if (intersects(roomNbrs, walls[i]) && !contains(wallNbrs[i], room)) {
                        candidate = false;
                        break;
                    }
                }
                if (!candidate) continue;

                // Now we have a valid candidate. Add it to our list.
#ifdef DEBUG
                cout << "CANDIDATE FOUND:" << endl;
                cout << "\troom: " << room << endl;
                cout << "\twalls: ";
                for (auto w: walls) {
                    cout << '[';
                    for (auto i=0; i < w.size(); ++i) {
                        cout << w[i];
                        if (i < w.size() - 1)
                            cout << ',';
                    }
                    cout << "] ";
                }
                cout << endl;
#endif
                CandidateConfiguration c { room, walls };
                configurations.emplace_back(c);
                if (aborter(c)) {
                    aborted = true;
                    break;
                }
            }
        } catch (std::overflow_error &) {}

        if (aborted)
            break;
    }

    return configurations;
}

GridColouring::Neighbourhood GridColouring::neighbourhood(const GridColouring::ColourCollection &colours) const {
    Neighbourhood nbr;

    for (const auto c: colours) {
        const auto[x, y] = reverseLookup[c];
        nbr.emplace_back(cellColour(x - 1, y));
        nbr.emplace_back(cellColour(x + 1, y));
        nbr.emplace_back(cellColour(x, y - 1));
        nbr.emplace_back(cellColour(x, y + 1));
    }

    return nbr;
}

bool GridColouring::isContiguous(const GridColouring::ColourCollection &colours) const {
    // Keep track of the cells that we've visited.
    std::map<int, bool> visited;

    // Put the first cell in the queue.
    std::queue<Colour> cellsToProcess;
    cellsToProcess.push(colours[0]);
    while (!cellsToProcess.empty()) {
        auto cell = cellsToProcess.front();
        cellsToProcess.pop();
        visited[cell] = true;

        auto nbrs = neighbourhood(ColourCollection{cell});
        for (auto n: nbrs)
            if (visited.find(n) == visited.end() && contains(colours, n))
                cellsToProcess.push(n);
    }

    // We can tell purely on size if every cell was visited.
    return visited.size() == colours.size();
}