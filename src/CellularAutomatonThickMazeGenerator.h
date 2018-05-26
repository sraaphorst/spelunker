/**
 * CellularAutomatonThickMazeGenerator.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * A 2D cellular automaton with specified birth and survival patterns with a given
 * neighbourhood type.
 */

#ifndef SPELUNKER_CELLULARAUTOMATONTHICKMAZEGENERATOR_H
#define SPELUNKER_CELLULARAUTOMATONTHICKMAZEGENERATOR_H

#include <functional>

#include "ThickMazeAttributes.h"
#include "ThickMazeGenerator.h

namespace spelunker::thickmaze {
    class CellularAutomatonThickMazeGenerator final : ThickMazeGenerator {
    public:
        /// The type of neighbourhood to use for the cellular automaton.
        /**
         * The type of neighbourhood to consider in the cellular automaton.
         * 
         */
        enum NeighbourhoodType {
            MOORE,
            VON_NEUMANN
        };

    private:
        std::function<int()
    };
};

#endif //SPELUNKER_CELLULARAUTOMATONTHICKMAZEGENERATOR_H
