/**
 * RandomizedKruskalMaze.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <vector>
#include <boost/pending/disjoint_sets.hpp>

#include <types/CommonMazeAttributes.h>
#include <types/Dimensions2D.h>
#include <types/DisjointSetHelper.h>
#include <math/RNG.h>

#include "Maze.h"
#include "MazeAttributes.h"
#include "MazeGenerator.h"
#include "KruskalMazeGenerator.h"

namespace spelunker::maze {
    using namespace boost;

    KruskalMazeGenerator::KruskalMazeGenerator(const types::Dimensions2D &d)
        : MazeGenerator{d} {}

    KruskalMazeGenerator::KruskalMazeGenerator(const int w, const int h)
        : KruskalMazeGenerator{types::Dimensions2D{w, h}} {}

    const Maze KruskalMazeGenerator::generate() const {
        const auto [width, height] = getDimensions().values();

        // We start with all walls, and then remove them iteratively.
        auto wi = initializeEmptyLayout(true);

        // Create a collection of all possible walls.
        std::vector<int> walls;
        const int wallUpper = getNumWalls();
        for (auto w = 0; w < wallUpper; ++w)
            walls.emplace_back(w);

        // Get the map of wall ranks to adjacent cells.
        auto unrank = createUnrankWallMap();

        // Given a wall, find its adjacent cell.
        // We need disjoint sets to represent the connected sets of cells.
        // To do so efficiently, we use Boost's disjoint_sets with some helper classes and methods.

        // Create a vector of all elements.
        std::vector<types::Element> elements;
        elements.reserve(width * height);
        for (auto x = 0; x < width; ++x)
            for (auto y = 0; y < height; ++y)
                elements.emplace_back(types::Element(rankCell(x, y)));

        for (auto i = 0; i < elements.size(); ++i)
            elements[i].dsID = i;

        // Create disjoint singleton sets.
        types::Rank rank(elements);
        types::Parent parent(elements);
        boost::disjoint_sets<types::Rank *, types::Parent *> dsets(&rank, &parent);
        for (auto &e: elements)
            dsets.make_set(e);

        // Shuffle the vector of walls and then iterate over them.
        math::RNG::shuffle(walls);

        for (auto w : walls) {
            const auto &pp = unrank[w];

            const auto [c1, c2] = pp;

            const auto [cx1, cy1] = c1.first;
            const auto cr1 = rankCell(cx1, cy1);

            const auto [cx2, cy2] = c2.first;
            const auto cr2 = rankCell(cx2, cy2);

            // If the cells belong to separate partitions, remove the wall and join them.
            const auto &set1 = dsets.find_set(elements[cr1]);
            const auto &set2 = dsets.find_set(elements[cr2]);
            if (set1 != set2) {
                wi[w] = false;
                dsets.link(set1, set2);
            }
        }

        return Maze(getDimensions(), wi);
    }
}