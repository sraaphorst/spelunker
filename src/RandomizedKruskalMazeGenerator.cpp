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

#include <iostream>

#include "DisjointSetHelper.h"
#include "Maze.h"
#include "RandomizedKruskalMazeGenerator.h"

namespace vorpal::maze {
    using namespace std;

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
        cout << "Creating w=" << width << " x h=" << height << " total=" << (width * height) << endl;
        elements.reserve(width * height);
        for (auto x = 0; x < width; ++x)
            for (auto y = 0; y < height; ++y) {
                elements.emplace_back(Element(ranker(x, y)));
            }

        cout << "Created " << elements.size() << " elements." << endl;
        for (auto i=0; i < elements.size(); ++i)
            elements[i].dsID = i;

        // Create disjoint singleton sets.
        Rank rank(elements);
        Parent parent(elements);
        boost::disjoint_sets<Rank *, Parent *> dsets(&rank, &parent);
        for (auto &e: elements)
            dsets.make_set(e);
//        std::for_each(elements.begin(), elements.end(), [&dsets](auto e) { dsets.make_set(e); });

        dsets.compress_sets(elements.begin(), elements.end());
        printElements(elements);

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

            cout << "C1: (" << cx1 << "," << cy1 << ") - (" << cx2 << "," << cy2 << "), r1=" << cr1 << ", r2=" << cr2 << endl;

            // If the cells belong to separate partitions, remove the wall and join them.
            const auto &set1 = dsets.find_set(elements.at(cr1));
            const auto &set2 = dsets.find_set(elements.at(cr2));
//            cout << "cr1=" << cr1 << ", cr2= " << cr2 << ", rk1=" << set1.dsRank << ", rk2=" << set2.dsRank
//                 << ", id1=" << set1.dsID << ", id2=" << set2.dsID << endl;
            if (set1 != set2) {
                cout << "Removing wall " << w << ": (" << cx1 << "," << cy1 << ") - (" << cx2 << "," << cy2 << ")\n";
                wi[w] = false;
                dsets.link(set1, set2);
                cout << "Done" << endl;
                //dsets.union_set(elements[cr1], elements[cr2]);
            }
        }

        printElements(elements);

        return Maze(width, height, wi);
    }
}