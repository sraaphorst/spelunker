/**
 * RandomizedKruskalMaze.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <algorithm>
#include <set>
#include <tuple>
#include <vector>
#include <boost/pending/disjoint_sets.hpp>
#include "DisjointSetHelper.h"
#include "Maze.h"
#include "RandomizedKruskalMaze.h"

namespace vorpal {
    namespace maze {
        using namespace std;

        RandomizedKruskalMaze::RandomizedKruskalMaze(const int w,
                                                     const int h,
                                                     const vorpal::maze::Cell &s,
                                                     const vorpal::maze::CellCollection &ends)
        : Maze(w, h, s, ends) {}

        const WallIncidence RandomizedKruskalMaze::generate() {
            // We start with all walls, and then remove them iteratively.
            WallIncidence wi = initializeEmptyLayout(true);

            // Create a collection of all possible walls.
            // TODO: This is not going to work. We need walls so that we can determine the cells on either side of them.
            std::vector< int > walls;
            for (auto x = 0; x < width; ++x)
                for (auto y = 0; y < height; ++y)
                    for (auto d : Directions)
                        walls.emplace_back(rankPosition(pos(x, y, d)));

            // Given a wall, find its adjacent cell.
            // We need disjoint sets to represent the connected sets of cells.
            // To do so efficiently, we use Boost's disjoint_sets with some helper classes and methods.

            // Rank / unrank cells:
            auto ranker = [this](int x, int y) { return x * width + y; };
            auto unranker = [this](int rk) { return std::make_pair(rk / width, rk % width); };

            // Create a vector of all elements.
            std::vector< Element > elements;
            elements.reserve(width * height);
            for (int x=0; x < width; ++x)
                for (int y=0; y < width; ++y)
                    elements.emplace_back(Element(ranker(x, y)));

            // Create disjoint singleton sets.
            Rank rank(elements);
            Parent parent(elements);
            boost::disjoint_sets<Rank*, Parent*> dsets(&rank, &parent);
            std::for_each(elements.begin(), elements.end(), [&dsets](auto e) { dsets.make_set(e); });

            // Shuffle the vector of walls and then iterate over them.
            random_shuffle(walls.begin(), walls.end());

            // TODO: Fix and fill in rest.
            // Make the compiler happy for now.
            return wi;
        }
    }
}