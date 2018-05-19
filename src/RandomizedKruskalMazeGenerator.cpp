/**
 * RandomizedKruskalMaze.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <algorithm>
#include <random>
#include <tuple>
#include <vector>
#include <boost/pending/disjoint_sets.hpp>

#include "DisjointSetHelper.h"
#include "Maze.h"
#include "RandomizedKruskalMazeGenerator.h"

namespace vorpal::maze {
    RandomizedKruskalMazeGenerator::RandomizedKruskalMazeGenerator(const int w, const int h)
            : MazeGenerator(w, h) {}

    const Maze RandomizedKruskalMazeGenerator::generate() {
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

        // Rank cells:
        const auto ranker = [this](int x, int y) { return y * width + x; };

        // Create a vector of all elements.
        std::vector<Element> elements;
        elements.reserve(width * height);
        for (auto x = 0; x < width; ++x)
            for (auto y = 0; y < height; ++y)
                elements.emplace_back(Element(ranker(x, y)));

        for (auto i=0; i < elements.size(); ++i)
            elements[i].dsID = i;

        // Create disjoint singleton sets.
        Rank rank(elements);
        Parent parent(elements);
        boost::disjoint_sets<Rank *, Parent *> dsets(&rank, &parent);
        for (auto &e: elements)
            dsets.make_set(e);

        // Shuffle the vector of walls and then iterate over them.
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(walls.begin(), walls.end(), g);

        for (auto w : walls) {
            const auto &pp = unrank[w];

            const auto &c1 = pp.first.first;
            const auto &cx1 = c1.first;
            const auto &cy1 = c1.second;
            const auto cr1 = ranker(cx1, cy1);

            const auto &c2 = pp.second.first;
            const auto &cx2 = c2.first;
            const auto &cy2 = c2.second;
            const auto cr2 = ranker(cx2, cy2);

            // If the cells belong to separate partitions, remove the wall and join them.
            const auto &set1 = dsets.find_set(elements.at(cr1));
            const auto &set2 = dsets.find_set(elements.at(cr2));
            if (set1 != set2) {
                wi[w] = false;
                dsets.link(set1, set2);
            }
        }

        return Maze(width, height, wi);
    }
}