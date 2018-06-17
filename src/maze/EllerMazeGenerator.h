/**
 * EllerMazeGenerator.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * A maze generator using
 * <a href="http://weblog.jamisbuck.org/2010/12/29/maze-generation-eller-s-algorithm">
 * Eller's algorithm.</a>
 */

#pragma once

#include <types/Dimensions2D.h>

#include "MazeGenerator.h"

namespace spelunker::maze {
    class Maze;

    /// Maze generator using Eller's algorithm.
    /**
     * This is a maze generator using Eller's algorithm.
     *
     * The idea here is to initialize the maze with all walls, putting each cell in a disjoint set.
     *
     * We begin in the upper left corner, and continue to carve east with probability p, joining the cells together
     * to represent a larger set.
     *
     * When we no longer carve east (either because the probability failed, or we reach the end of the row), we
     * go over the contiguous cells and carve one or more southern walls.
     *
     * Here, we allow a fractional parameter d, which indicates the maximum "density" of southward carving.
     * The algorithm then carves between 1 and min(1, d * size of contiguous cells) southern walls amongst the
     * contiguous cells, adding them to the set.
     *
     * Then proceed with the next row and continue iteratively until the last row is reached.
     * In the last row, carve walls between cells that are still in disjoint sets to ensure the maze is connected.
     * This will always generate a perfect maze.
     *
     * The parameters p and d, which both default to 0.5, can be played with to result in mazes with a horizontal or
     * vertical bias, i.e. long horizontal or vertical passages.
     * Note that, for low p, you get long, vertical corridors, since there is little horizontal interaction.
     * With high p and low d, you get long, horizontal corridors.
     *
     * This can be done to absurdity: for example, with p = 0.0001 and any value of d, any maze with less than
     * 10,000 cells will have an expected value of zero for number of horizontal branches, and thus, the entire
     * maze will consist of columns joined together by a single bottom row.
     */
    class EllerMazeGenerator final : public MazeGenerator {
    public:
        /// Maze generator using Eller's algorithm that generates walls with probability p and density d.
        /**
         * A maze generator using Eller's algorithm that generates horizontal gaps with probability p
         * and generates vertical gaps with density up to d.
         *
         * @param d dimensions of the maze
         * @param p probability with which to adjoin two adjacent cells in different sets
         * @param den density of vertical adjoining (i.e. if we have c cells in a set, a random
         *          number of vertical connections in [1,min(1, c * d)] will be made.
         */
        EllerMazeGenerator(const types::Dimensions2D &d, double p, double den);

        /// Maze generator using Eller's algorithm that generates walls with probability p and density d.
        /**
         * A maze generator using Eller's algorithm that generates horizontal gaps with probability p
         * and generates vertical gaps with density up to d.
         *
         * @param w width of the maze
         * @param h height of the maze
         * @param p probability with which to adjoin two adjacent cells in different sets
         * @param den density of vertical adjoining (i.e. if we have c cells in a set, a random
         *          number of vertical connections in [1,min(1, c * d)] will be made.
         */
        EllerMazeGenerator(int w, int h, double p, double den);

        /**
         * Create a maze generator using Eller's algorithm that generates walls with probability 0.5
         * and generates vertical gaps with density up to 0.5.
         * @param d dimensions of the maze
         */
        EllerMazeGenerator(const types::Dimensions2D &d);

        /**
         * Create a maze generator using Eller's algorithm that generates walls with probability 0.5
         * and generates vertical gaps with density up to 0.5.
         * @param w width of the maze
         * @param h height of the maze
         */
        EllerMazeGenerator(int w, int h);

        ~EllerMazeGenerator() final = default;

        const Maze generate() const noexcept final;

        static constexpr double defaultProbability = 0.5;
        static constexpr double defaultDensity = 0.5;

    private:
        const double probability;
        const double density;
    };
};
