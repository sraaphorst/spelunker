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

namespace spelunker::thickmaze {
    static int moore(const types::Cell c, const types::CellContents &cs) {
        int alive = 0;
        const int maxRow = cs[0].size() - 1;
        const int maxCol = cs.size() - 1;

        auto[x, y] = c;

        // Covers the entire west column. Assume border structure.
        if (x > 0) {
            if (y > 0 && cs[x - 1][y - 1] == types::WALL) ++alive;
            if (y < maxRow && cs[x - 1][y + 1] == types::WALL) ++alive;
            if (cs[x - 1][y] == types::WALL) ++alive;
        } //else alive += 3;

        // Covers the entire east column. Assume border wall.
        if (x < maxCol) {
            if (y > 0 && cs[x + 1][y - 1] == types::WALL) ++alive;
            if (y < maxRow && cs[x + 1][y + 1] == types::WALL) ++alive;
            if (cs[x + 1][y] == types::WALL) ++alive;
        } //else alive += 3;

        // Now cover the north and south cells. Assume border wall.
        if (y > 0) {
            if (cs[x][y - 1] == types::WALL) ++alive;
        } //else alive += 1;
        if (y < maxRow) {
            if (cs[x][y + 1] == types::WALL) ++alive;
        } //else alive += 1;

        return alive;
    }

    static int vonNeumann(const types::Cell c, const types::CellContents &cs) {
        int alive = 0;
        const int maxRow = cs[0].size() - 1;
        const int maxCol = cs.size() - 1;

        auto[x, y] = c;

        // West segment.
        if (x > 1 && cs[x - 2][y] == types::WALL) ++alive;
        if (x == 0 || cs[x - 1][y] == types::WALL) ++alive;

        // East segment.
        if (x < maxCol - 1 && cs[x + 2][y] == types::WALL) ++alive;
        if (x == maxCol || cs[x + 1][y] == types::WALL) ++alive;

        // North segment.
        if (y > 1 && cs[2][y - 2] == types::WALL) ++alive;
        if (y == 0 || cs[x][y - 1] == types::WALL) ++alive;

        // South segment.
        if (y < maxRow - 1 && cs[x][y + 2] == types::WALL) ++alive;
        if (y == maxRow || cs[x][y + 1] == types::WALL) ++alive;

        return alive;
    }

    CellularAutomatonThickMazeGenerator::NeighbourCounter CellularAutomatonThickMazeGenerator::fromNeighbourhoodType(NeighbourhoodType n) {
        switch (n) {
            case MOORE:
                return &moore;
            case VON_NEUMANN:
                return &vonNeumann;
        }
    }


    CellularAutomatonThickMazeGenerator::DetermineBehaviour CellularAutomatonThickMazeGenerator::fromAlgorithm(Algorithm a) {
        switch (a) {
            case MAZECETRIC:
                return [](const int num) {
                    if (num >= 1 && num <= 4) return SURVIVE;
                    else if (num == 3)
                        return BORN;
                    else return DIE;
                };
            case MAZE:
                return [](const int num) {
                    if (num >= 1 && num <= 5) return SURVIVE;
                    else if (num == 3)
                        return BORN;
                    else return DIE;
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
                    Behaviour b = st.determineBehaviour(numNbrs);
                    if (b == SURVIVE) newContents[x][y] = contents[x][y];
                    else if (b == BORN) newContents[x][y] = types::WALL;
                }

            contents = newContents;
        }

        return ThickMaze(width, height, contents);
    }
}
