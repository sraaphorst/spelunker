/**
 * EllerMazeGenerator.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <cmath>
#include <vector>
#include <boost/pending/disjoint_sets.hpp>

#include "DisjointSetHelper.h"
#include "Maze.h"
#include "MazeAttributes.h"
#include "MazeGenerator.h"
#include "RNG.h"
#include "EllerMazeGenerator.h"

namespace spelunker::maze {
    using namespace boost;

    EllerMazeGenerator::EllerMazeGenerator(const int w, const int h, const double p, const double d)
            : MazeGenerator(w, h), probability(p), density(d) {}

    EllerMazeGenerator::EllerMazeGenerator(int w, int h)
            : MazeGenerator(w, h), probability(0.5), density(0.5) {}

    const Maze EllerMazeGenerator::generate() {
        // We start with all walls, and then remove them iteratively.
        auto wi = initializeEmptyLayout(true);

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

        for (int i = 0; i < width; ++i)
            for (int j = 0; j < width; ++j) {
                const auto m = rankCell(i, j);
                const auto n = unrankCell(m);
                assert(n.first == i);
                assert(n.second == j);
            }
        const auto wMax = width - 1;
        for (auto y = 0; y < height; ++y) {
            for (auto x = 0; x < wMax; ++x) {
                // Check to see if (x,y) and (x+1,y) are in the same set.
                // If not, connect them with the given probability, or, in
                // the case of the last row, connect them anyway.
                const auto &set1 = dsets.find_set(elements[rankCell(x, y)]);
                const auto &set2 = dsets.find_set(elements[rankCell(x + 1, y)]);

                if (set1 != set2 && (y == height - 1 || math::RNG::randomProbability() < probability)) {
                    wi[rankPos(pos(x, y, EAST))] = false;
                    dsets.link(set1, set2);
                }
            }

            // If we are not at the bottom row, we find the partitioning and create at least one down wall per partition.
            // The idea here is to iterate over the row, collecting up the cells that are connected.
            // We then remove south-facing walls: at least one for each connected set.
            if (y != height - 1) {
                const auto firstRk = rankCell(0, y);
                auto curRep = dsets.find_set(elements[firstRk]);
                std::vector<int> cells;
                cells.emplace_back(firstRk);

                // This is a bit hacky: we go until width, so that we process the last cell if
                // it isn't connected to the previous cells in the row, since it would have
                // been added to freshly cleared cells in the previous iteration.
                for (auto x = 1; x <= width; ++x) {
                    // If this cell has the same representative as the previous, add it to the vector
                    // and continue if we are not at the end of the row.
                    // Since we go until width, we have to deal with the case that x == width.
                    // We just hackishly take the rank of the previous cell to get something valid
                    // in nRep for the set we're looking at.
                    const auto rk = rankCell(x < width ? x : x - 1, y);
                    const auto nRep = dsets.find_set(elements[rk]);

                    if (x < width && nRep == curRep) {
                        cells.emplace_back(rk);
                        continue;
                    }

                    // Otherwise, we process the set, removing vertical walls as per the density.
                    const auto maxNumGaps = std::max(1, (int) (density * cells.size()));
                    auto numGapsToMake = math::RNG::randomRange(maxNumGaps) + 1;

                    // Select that many cells and remove their south walls.
                    while (numGapsToMake > 0) {
                        const auto idx = math::RNG::randomRange(cells.size());
                        std::swap(cells[idx], cells.back());
                        const auto sRk = cells.back();
                        cells.pop_back();

                        const auto cell = unrankCell(sRk);
                        wi[rankPos(pos(cell.first, cell.second, SOUTH))] = false;

                        // Add the cell to this set in the partition.
                        const auto &setDown = dsets.find_set(elements[rankCell(cell.first, cell.second + 1)]);
                        dsets.link(curRep, setDown);
                        --numGapsToMake;
                    }

                    // Now update the data structures.
                    // We start a fresh partition. If we are at the end and the last cell in the
                    // row was connected to the previous cell, the algorithm will terminate after
                    // this, so emplacing rk does nothing it shouldn't.
                    curRep = nRep;
                    cells.clear();
                    cells.emplace_back(rk);
                }
            }
        }

        return Maze(width, height, wi);
    }
}