/**
 * GrowingTreeMazeGeneration.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * A maze generator using the
 * <a href="http://weblog.jamisbuck.org/2011/1/27/maze-generation-growing-tree-algorithm">
 * growing tree algorithm.</a>
 */

#pragma once

#include <functional>

#include "types/CommonMazeAttributes.h"
#include "MazeAttributes.h"
#include "MazeGenerator.h"

namespace spelunker::maze {
    class Maze;

    /**
     * The growing tree maze generator is incredibly simplistic in its concept:
     *
     * Setup:
     * Let C be an initially empty collection of cells.
     * Pick a cell c at random and put it into C.
     *
     * Algorithm:
     * While C is nonempty:
     * Choose any cell in C, say c.
     * If c has no unvisited neighbours, remove c from C and loop.
     * If c has unvisited neighbours, pick one at random, say c'.
     *   Carve a passage from c to c' and add c' to C.
     *
     * The interesting part of this algorithm is the selection mechanism used to select a cell in C.
     * This class provides four possible selection strategies namely:
     * - OLDEST: pick the oldest cell first
     * - NEWEST: pick the newest cell first (equivalent to randomized depth-first search)
     * - RANDOM: pick a random cell (equivalent to randomized Prim's)
     * - MIDDLE: pick the middle cell first.
     *
     * but also allows the user to implement their own by supplying a function:
     *
     * \code
     * int (*)(const CellContainer &C)
     * \endcode
     *
     * that is called with C nonempty and expects the user to return the index of the candidate c in C.
     *
     * Note that elements are emplaced at the back of C, so 0 is the oldest element, and C.size() - 1
     * is the newest.
     */
    class GrowingTreeMazeGenerator final : public MazeGenerator {
    public:
        using Selector = std::function<int(const types::CellCollection&)>;
        /// The predetermined cell selection strategies.
        enum CellSelectionStrategy {
            OLDEST = 0,
            NEWEST,
            MIDDLE,
            RANDOM
        };

        /// Create a growing tree maze generator with a predetermined cell selection strategy.
        /**
         * Create a growing tree maze generator with a predetermined cell selection strategy.
         * @param w width of mazes to produce
         * @param h height of mazes to produce
         * @param strategy the strategy to use when picking a cell from C
         */
        GrowingTreeMazeGenerator(int w, int h, const CellSelectionStrategy &strategy);


        /// Create a growing tree maze generator with a user-specified selector.
        /**
         * Create a growing tree maze generator with a user-specified selector.
         * @param w width of mazes to produce
         * @param h height of mazes to produce
         * @param sel the cell selector, which should return the index of the cell to use
         */
        GrowingTreeMazeGenerator(int w, int h, Selector sel);

        ~GrowingTreeMazeGenerator() final = default;

        const Maze generate() final;

    private:
        /// The selection function. Should return an index of the CellCollection. Only called if it contains elements.
        const Selector selector;

        /// Given a CellSelectionStrategy, return its selector.
        static Selector getSelector(CellSelectionStrategy css);
    };
}

