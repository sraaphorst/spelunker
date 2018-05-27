/**
 * CellularAutomatonThickMazeGenerator.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include "CommonMazeAttributes.h"
#include "RNG.h"
#include "ThickMaze.h"
#include "ThickMazeAttributes.h"
#include "ThickMazeGenerator.h"
#include "CellularAutomatonThickMazeGenerator.h"

#include <iostream>
using namespace std;

namespace spelunker::thickmaze {
    static int wrap(int max, int x) {
        if (x < 0) return max + x + 1;
        if (x > max) return x % (max + 1);
        else return x;

    }
    static int moore(const types::Cell c, const types::CellContents &cs) {
        int alive = 0;
        const int maxRow = cs[0].size() - 1;
        const int maxCol = cs.size() - 1;

        auto[x, y] = c;
        const int xm1 = wrap(maxCol, x-1);
        const int xp1 = wrap(maxCol, x+1);
        const int ym1 = wrap(maxRow, y-1);
        const int yp1 = wrap(maxRow, y+1);

        // Covers the entire west column. Assume toroidal structure.
        if (cs[xm1][ym1] == types::WALL) ++alive;
        if (cs[xm1][y]   == types::WALL) ++alive;
        if (cs[xm1][yp1] == types::WALL) ++alive;

        // Covers the entire east column. Assume toroidal structure.
        if (cs[xp1][ym1] == types::WALL) ++alive;
        if (cs[xp1][y]   == types::WALL) ++alive;
        if (cs[xp1][yp1] == types::WALL) ++alive;

        // Cover the north and south cells.
        if (cs[x][ym1]   == types::WALL) ++alive;
        if (cs[x][yp1]   == types::WALL) ++alive;

        return alive;
    }

    static int vonNeumann(const types::Cell c, const types::CellContents &cs) {
        int alive = 0;
        const int maxRow = cs[0].size() - 1;
        const int maxCol = cs.size() - 1;

        auto[x, y] = c;
        const int xm1 = wrap(maxCol, x-1);
        const int xm2 = wrap(maxCol, x-2);
        const int xp1 = wrap(maxCol, x+1);
        const int xp2 = wrap(maxCol, x+2);
        const int ym1 = wrap(maxRow, y-1);
        const int ym2 = wrap(maxRow, y-2);
        const int yp1 = wrap(maxRow, y+1);
        const int yp2 = wrap(maxRow, y+2);

        // West segment.
        if (cs[xm2][y] == types::WALL) ++alive;
        if (cs[xm1][y] == types::WALL) ++alive;

        // East segment
        if (cs[xp1][y] == types::WALL) ++alive;
        if (cs[xp2][y] == types::WALL) ++alive;

        // North segment
        if (cs[x][ym1] == types::WALL) ++alive;
        if (cs[x][ym2] == types::WALL) ++alive;

        // South segment
        if (cs[x][yp1] == types::WALL) ++alive;
        if (cs[x][yp2] == types::WALL) ++alive;

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
            case MAZECETRIC:
                // B3/S1234
                return [](const int num, const types::CellType ct) {
                    if (num >= 1 && num <= 4 && ct == types::WALL) return SURVIVE;
                    if (num == 3 && ct == types::FLOOR) return BORN;
                    return DIE;
                };
            case MAZE:
                // B3/S12345
                return [](const int num, const types::CellType ct) {
                    if (num >= 1 && num <= 5 && ct == types::WALL) return SURVIVE;
                    if (num == 3 && ct == types::FLOOR) return BORN;
                    return DIE;
                };
            case VOTE45:
                // B4678/S35678: This algorithm seems flawed, as it fills up.
                // Recommended on: https://steamcommunity.com/app/357330/discussions/0/618459405722195374
                return [](const int num, const types::CellType ct) {
                    if ((num == 3 || (num >=5 && num <=8)) && ct == types::WALL) return SURVIVE;
                    if (num == 4 || (num >= 6 && num <= 8) && ct == types::FLOOR) return BORN;
                    return DIE;
                };
            case VOTE:
                // B5678/S45678: This algorithm produces cavernous rooms.
                // Recommended on: https://steamcommunity.com/app/357330/discussions/0/618459405722195374
                return [](const int num, const types::CellType ct) {
                    if (num >= 4 && num <= 8 && ct == types::WALL) return SURVIVE;
                    if (num >= 5 && num <= 8 && ct == types::FLOOR) return BORN;
                    return DIE;
                };
        }
    }

    CellularAutomatonThickMazeGenerator::CellularAutomatonThickMazeGenerator(int w, int h, settings s)
            : ThickMazeGenerator(w, h), st(s) {}

    const ThickMaze CellularAutomatonThickMazeGenerator::generate() {
        // Create and initialize the cell contents.
        types::CellContents contents = createEmptyContents();

        // Create the random initialization.
        for (auto y = 0; y < height; ++y)
            for (auto x = 0; x < width; ++x)
                if (math::RNG::randomProbability() < st.probability)
                    contents[x][y] = types::WALL;

        // Run the algorithm for the desired number of iterations.
        for (auto i = 0; i < st.numGenerations; ++i) {
            // Create a new contents to initialize.
            types::CellContents newContents = createEmptyContents();

            // Iterate over each cell and determine its state.
            for (auto y = 0; y < height; ++y)
                for (auto x = 0; x < width; ++x) {
                    // Count the neighbours for this cell.
                    int numNbrs = st.neighbourCounter(types::cell(x, y), contents);

                    // Determine the behaviour of this cell.
                    // As newContents was initialized to all floor, we can ignore death.
                    Behaviour b = st.determineBehaviour(numNbrs, contents[x][y]);
                    if (b == SURVIVE) newContents[x][y] = contents[x][y];
                    else if (b == BORN) newContents[x][y] = types::WALL;
                }

            contents = newContents;
        }

        return ThickMaze(width, height, contents);
    }
}
