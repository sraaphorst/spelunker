/**
 * RandomizedKruskalMaze.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <vector>
#include <boost/pending/disjoint_sets.hpp>

#include "DisjointSetHelper.h"
#include "Maze.h"
#include "MazeAttributes.h"
#include "MazeGenerator.h"
#include "RNG.h"
#include "KruskalMazeGenerator.h"

namespace spelunker::maze {
    KruskalMazeGenerator::KruskalMazeGenerator(const int w, const int h)
            : MazeGenerator(w, h) {}

    const Maze KruskalMazeGenerator::generate() {
        // We start with all walls, and then remove them iteratively.
        auto wi = initializeEmptyLayout(true);

        // Create a collection of all possible walls.
        std::vector<int> walls;
        for (auto w = 0; w < numWalls; ++w)
            walls.emplace_back(w);

        // Get the map of wall ranks to adjacent cells.
        auto unrank = createUnrankWallMap();

        // Given a wall, find its adjacent cell.
        // We need disjoint sets to represent the connected sets of cells.
        // To do so efficiently, we use Boost's disjoint_sets with some helper classes and methods.

        // Create a vector of all elements.
        std::vector<Element> elements;
        elements.reserve(width * height);
        for (auto x = 0; x < width; ++x)
            for (auto y = 0; y < height; ++y)
                elements.emplace_back(Element(rankCell(x, y)));

        for (auto i = 0; i < elements.size(); ++i)
            elements[i].dsID = i;

        // Create disjoint singleton sets.
        Rank rank(elements);
        Parent parent(elements);
        boost::disjoint_sets<Rank *, Parent *> dsets(&rank, &parent);
        for (auto &e: elements)
            dsets.make_set(e);

        // Shuffle the vector of walls and then iterate over them.
        math::RNG::shuffle(walls);

        for (auto w : walls) {
            const auto &pp = unrank[w];

            const auto& [cx1, cy1] = pp.first.first;
            const auto cr1 = rankCell(cx1, cy1);

            const auto& [cx2, cy2] = pp.second.first;
            const auto cr2 = rankCell(cx2, cy2);

            // If the cells belong to separate partitions, remove the wall and join them.
            const auto &set1 = dsets.find_set(elements[cr1]);
            const auto &set2 = dsets.find_set(elements[cr2]);
            if (set1 != set2) {
                wi[w] = false;
                dsets.link(set1, set2);
            }
        }

        return Maze(width, height, wi);
    }
}