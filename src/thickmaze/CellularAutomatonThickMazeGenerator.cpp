/**
 * CellularAutomatonThickMazeGenerator.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <list>

#include "types/CommonMazeAttributes.h"
#include "math/RNG.h"
#include "ThickMaze.h"
#include "ThickMazeAttributes.h"
#include "ThickMazeGenerator.h"
#include "CellularAutomatonThickMazeGenerator.h"

namespace spelunker::thickmaze {
    static int wrap(int max, int x) {
        if (x < 0) return max + x + 1;
        if (x > max) return x % (max + 1);
        else return x;

    }
    static int moore(const types::Cell c, const CellContents &cs) {
        int alive = 0;
        const auto maxRow = cs[0].size() - 1;
        const auto maxCol = cs.size() - 1;

        auto[x, y] = c;
        const auto xm1 = wrap(maxCol, x-1);
        const auto xp1 = wrap(maxCol, x+1);
        const auto ym1 = wrap(maxRow, y-1);
        const auto yp1 = wrap(maxRow, y+1);

        // Covers the entire west column. Assume toroidal structure.
        if (cs[xm1][ym1] == WALL) ++alive;
        if (cs[xm1][y]   == WALL) ++alive;
        if (cs[xm1][yp1] == WALL) ++alive;

        // Covers the entire east column. Assume toroidal structure.
        if (cs[xp1][ym1] == WALL) ++alive;
        if (cs[xp1][y]   == WALL) ++alive;
        if (cs[xp1][yp1] == WALL) ++alive;

        // Cover the north and south cells.
        if (cs[x][ym1]   == WALL) ++alive;
        if (cs[x][yp1]   == WALL) ++alive;

        return alive;
    }

    static int vonNeumann(const types::Cell c, const CellContents &cs) {
        int alive = 0;
        const auto maxRow = cs[0].size() - 1;
        const auto maxCol = cs.size() - 1;

        auto[x, y] = c;
        const auto xm1 = wrap(maxCol, x-1);
        const auto xm2 = wrap(maxCol, x-2);
        const auto xp1 = wrap(maxCol, x+1);
        const auto xp2 = wrap(maxCol, x+2);
        const auto ym1 = wrap(maxRow, y-1);
        const auto ym2 = wrap(maxRow, y-2);
        const auto yp1 = wrap(maxRow, y+1);
        const auto yp2 = wrap(maxRow, y+2);

        // West segment.
        if (cs[xm2][y] == WALL) ++alive;
        if (cs[xm1][y] == WALL) ++alive;

        // East segment
        if (cs[xp1][y] == WALL) ++alive;
        if (cs[xp2][y] == WALL) ++alive;

        // North segment
        if (cs[x][ym1] == WALL) ++alive;
        if (cs[x][ym2] == WALL) ++alive;

        // South segment
        if (cs[x][yp1] == WALL) ++alive;
        if (cs[x][yp2] == WALL) ++alive;

        return alive;
    }

    CellularAutomatonThickMazeGenerator::NeighbourCounter CellularAutomatonThickMazeGenerator::fromNeighbourhoodType(NeighbourhoodType n) {
        switch (n) {
            case MOORE:
                return &moore;
            case VON_NEUMANN_EXTENDED:
                return &vonNeumann;
        }
    }


    CellularAutomatonThickMazeGenerator::DetermineBehaviour CellularAutomatonThickMazeGenerator::fromAlgorithm(Algorithm a) {
        switch (a) {
            case MAZECTRIC:
                // B3/S1234
                return [](const int num, const CellType ct) {
                    if (num >= 1 && num <= 4 && ct == WALL) return SURVIVE;
                    if (num == 3 && ct == FLOOR) return BORN;
                    return DIE;
                };
            case MAZE:
                // B3/S12345
                return [](const int num, const CellType ct) {
                    if (num >= 1 && num <= 5 && ct == WALL) return SURVIVE;
                    if (num == 3 && ct == FLOOR) return BORN;
                    return DIE;
                };
            case VOTE45:
                // B4678/S35678: This algorithm seems flawed, as it fills up.
                // Recommended on: https://steamcommunity.com/app/357330/discussions/0/618459405722195374
                return [](const int num, const CellType ct) {
                    if ((num == 3 || (num >=5 && num <=8)) && ct == WALL) return SURVIVE;
                    if ((num == 4 || (num >= 6 && num <= 8)) && ct == FLOOR) return BORN;
                    return DIE;
                };
            case VOTE:
                // B5678/S45678: This algorithm produces cavernous rooms.
                // Recommended on: https://steamcommunity.com/app/357330/discussions/0/618459405722195374
                return [](const int num, const CellType ct) {
                    if (num >= 4 && num <= 8 && ct == WALL) return SURVIVE;
                    if (num >= 5 && num <= 8 && ct == FLOOR) return BORN;
                    return DIE;
                };
            case B2S123:
                // B2/S123
                // Recommended on: https://english.rejbrand.se/rejbrand/article.asp?ItemIndex=421
                return [](const int num, const CellType ct) {
                    if (num >= 1 && num <= 3 && ct == WALL) return SURVIVE;
                    if (num == 2 && ct == FLOOR) return BORN;
                    return DIE;
                };
        }
    }

    CellularAutomatonThickMazeGenerator::CellularAutomatonThickMazeGenerator(int w, int h, settings &s)
            : ThickMazeGenerator(w, h), st(s) {}

    const ThickMaze CellularAutomatonThickMazeGenerator::generate() {
        // Create and initialize the cell contents.
        auto contents = createThickMazeCellContents(width, height);

        // The back-check chart.
        std::list<CellContents> prevs;

        // Create the random initialization.
        for (auto y = 0; y < height; ++y)
            for (auto x = 0; x < width; ++x)
                if (math::RNG::randomProbability() < st.probability)
                    contents[x][y] = WALL;
        prevs.emplace_back(contents);

        // Run the algorithm for the desired number of iterations unless stability is first achieved.
        auto inconsistent = true;

        for (auto i = 0; i < st.numGenerations && inconsistent; ++i) {
            // Create a new contents to initialize.
            auto newContents = createThickMazeCellContents(width, height);

            // Get the old contents from which to work.
            const auto &oldContents = prevs.back();

            // Iterate over each cell and determine its state.
            for (auto y = 0; y < height; ++y)
                for (auto x = 0; x < width; ++x) {
                    // Count the neighbours for this cell.
                    int numNbrs = st.neighbourCounter(types::cell(x, y), oldContents);

                    // Determine the behaviour of this cell.
                    // As newContents was initialized to all floor, we can ignore death.
                    Behaviour b = st.determineBehaviour(numNbrs, oldContents[x][y]);
                    if (b == SURVIVE) newContents[x][y] = oldContents[x][y];
                    else if (b == BORN) newContents[x][y] = WALL;
                }

            for (auto &prev : prevs) {
                if (newContents == prev) {
                    inconsistent = false;
                    break;
                }
            }
            contents = newContents;
            prevs.emplace_back(newContents);
            while (prevs.size() > st.stabilitySize)
                prevs.pop_front();
        }

        return ThickMaze(width, height, contents);
    }
}
