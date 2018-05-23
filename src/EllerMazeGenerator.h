/**
 * EllerMazeGenerator.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * A maze generator using
 * <a href="http://weblog.jamisbuck.org/2010/12/29/maze-generation-eller-s-algorithm">
 * Eller's algorithm.</a>
 */

#ifndef SPELUNKER_ELLERMAZEGENERATOR_H
#define SPELUNKER_ELLERMAZEGENERATOR_H

#include "MazeGenerator.h"

namespace vorpal::maze {
    class Maze;

    class EllerMazeGenerator final : public MazeGenerator {
    public:
        /// Create a maze generator using Eller's algorithm that generates walls with probability p.
        /**
         * Create a maze generator using Eller's algorithm that generates horizontal gaps with probability p
         * and generates vertical gaps with density up to d.
         *
         * Note that, for low p, you get long, vertical corridors, since there is little horizontal interaction.
         * With high p and low d, you get long, horizontal corridors.
         *
         * This can be done to absurdity: for example, with p = 0.0001 and any value of d, any maze with less than
         * 10,000 cells will have an expected value of zero for number of horizontal branches, and thus, the entire
         * maze will consist of columns joined together by a single bottom row.
         *
         * @param w width of the maze
         * @param h height of the maze
         * @param p probability with which to adjoin two adjacent cells in different sets
         * @param d density of vertical adjoining (i.e. if we have c cells in a set, a random
         *          number of vertical connections in [1,min(1, c * d)] will be made.
         */
        EllerMazeGenerator(int w, int h, double p, double d);

        /**
         * Create a maze generator using Eller's algorithm that generates walls with probability 0.5
         * and generates vertical gaps with density up to 0.5.
         * @param w width of the maze
         * @param h height of the maze
         */
        EllerMazeGenerator(int w, int h);

        virtual ~EllerMazeGenerator() = default;

        const Maze generate() override;

    private:
        const double probability;
        const double density;
    };
};

#endif //SPELUNKER_ELLERMAZEGENERATOR_H
