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

#include "CommonMazeAttributes.h"
#include "ThickMazeAttributes.h"
#include "ThickMazeGenerator.h"

namespace spelunker::thickmaze {
    class ThickMaze;

    /**
     * Cellular automata populate a grid randomly via a probability distribution, and then cells
     * go through a number of generations starting at time t=0, wherein the configuration at time
     * t=n deterministically generates the configuration at time t=n+1 according to a set of rules,
     * wherein the number of neighbours (for some defined type of neighbourhood) determines the
     * transition for each cell. Usually, a fairly steady state of stability is reached, perhaps
     * with some alternating back and forth between a small number of states.
     *
     * There is no guarantee that these mazes are perfect, or even connected: in fact, they
     * usually are not, and may contain large, open spaces; however, they are an interesting maze
     * generation technique in and of their own right, and could theoretically be used as starter
     * seeds for other algorithm.
     */
    class CellularAutomatonThickMazeGenerator final : ThickMazeGenerator {
    public:
        /// A function type that determines the number of living neighbours.
        using NeighbourCounter = std::function<int(const types::Cell, const types::CellContents&)>;

        /// The type of neighbourhood to use for the cellular automaton.
        /**
         * Predetermined types of neighbourhoods to consider in the cellular automaton.
         * The Moore neighbourhood comprises the eight cells directly around a cell.
         * The von Neumann neighbourhood comprises the eight cells of the orthogonal cross of length 2 around a cell.
         */
        enum NeighbourhoodType {
            MOORE,
            VON_NEUMANN_EXTENDED,
        };

        /// Possible outcomes for a cell at each round depending on the neighbours in its neighbourhood.
        enum Behaviour {
            BORN,
            SURVIVE,
            DIE,
        };

        /// A function type that determines the Behaviour of a cell given the number of living neighbours in its neighbourhood.
        using DetermineBehaviour = std::function<Behaviour(const int, const types::CellType)>;

        /// Convert one of the NeighbourhoodTypes into a NeighbourCounter.
        static NeighbourCounter fromNeighbourhoodType(NeighbourhoodType n);

        /**
         * Pre-existing algorithms determining behaviour of a cell.
         * They are to be interpreted as:
         * 1. Bb1b2b3... = If there are b1, b2, b3, etc. cells alive in the neighbourhood of a cell that does not
         *                   exit, then it is born.
         * 2. Ss1s2s3... = If there are s1, s2, s3, etc. cells alive in the neighbourhood of a cell that exists,
         *                   then it survives.
         * 3. Otherwise, the cell dies.
         *
         * Mazecetric has rule B3/S1234
         * Maze       has rule B3/S12345
         * Vote45     has rule B4678/S35678
         * Vote       has rule B5678/S45678
         */
        enum Algorithm {
            MAZECETRIC,
            MAZE,
            VOTE45,
            VOTE,
        };

        /// Convert one of the Algorithms into a DetermineBehaviour.
        static DetermineBehaviour fromAlgorithm(Algorithm a);

        /**
         * Cellular automata have lots of settings, so instead of providing a vast number of constructors, our
         * constructor accepts a struct of settings, which contain what we need.
         *
         * probability indicates the probability, during random seeding, that a cell should be marked as a wall.
         *    The default value is 0.5.
         *
         * numGenerations indicates the number of generations after random seeding that the automaton should produce.
         *    The default value is 10.
         *
         * neighbourhoodCounter determines how to count the surrounding neighbours of a cell.
         *    The default is using the Moore neighbourhood.
         *
         * determineBehaviour determines the behaviour of each cell with regards to the previous generation.
         *    The default is MAZE.
         *
         * The neighbourhoodCounter setting can be manually configured or configured from one of the NeighbourhoodType
         * values, MOORE or VON_NEUMANN_EXTENDED, by using the static fromNeighbourhoodType algorithm.
         *
         * The determineBehaviour setting can be manually configured or configured from one of the Algorithm values,
         * MAZECETRIC, MAZE, VOTE45, or VOTE by using the static fromAlgorithm method.
         */
        struct settings {
            double probability = 0.5;
            int numGenerations = 10000;
            NeighbourCounter neighbourCounter = fromNeighbourhoodType(MOORE);
            DetermineBehaviour determineBehaviour = fromAlgorithm(MAZE);
        };

        CellularAutomatonThickMazeGenerator(int w, int h, settings s);
        ~CellularAutomatonThickMazeGenerator() = default;

        const ThickMaze generate() override;
    private:
        /// The settings for the cellular automaton.
        settings st;
    };
};

#endif //SPELUNKER_CELLULARAUTOMATONTHICKMAZEGENERATOR_H
