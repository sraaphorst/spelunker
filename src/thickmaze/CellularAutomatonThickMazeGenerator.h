/**
 * CellularAutomatonThickMazeGenerator.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * A 2D cellular automaton with specified birth and survival patterns with a given
 * neighbourhood type.
 *
 * There is some interesting information here:
 * https://english.rejbrand.se/rejbrand/article.asp?ItemIndex=421
 */

#pragma once

#include <functional>

#include "types/CommonMazeAttributes.h"
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
     *
     * They tend to achieve stability very quickly, i.e. either reach a constant state that does not
     * change, or alternate between a small number of states.
     *
     * Note that these algorithms sometimes do better if the resultant mazes are reversed via a call
     * to @see{ThickMaze#reverse}.
     */
    class CellularAutomatonThickMazeGenerator final : ThickMazeGenerator {
    public:
        /// A function type that determines the number of living neighbours.
        using NeighbourCounter = std::function<int(const types::Cell, const CellContents&)>;

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

        /// A function type that determines the Behaviour of a cell given the number of living neighbours in its neighbourhood.
        /// Convert one of the NeighbourhoodTypes into a NeighbourCounter.
        static NeighbourCounter fromNeighbourhoodType(NeighbourhoodType n);

        /// Possible outcomes for a cell at each round depending on the neighbours in its neighbourhood.
        enum Behaviour {
            BORN,
            SURVIVE,
            DIE,
        };

        using DetermineBehaviour = std::function<Behaviour(const int, const CellType)>;

        /**
         * Pre-existing algorithms determining behaviour of a cell.
         * They are to be interpreted as:
         * 1. Bb1b2b3... = If there are b1, b2, b3, etc. cells alive in the neighbourhood of a cell that does not
         *                   exit, then it is born.
         * 2. Ss1s2s3... = If there are s1, s2, s3, etc. cells alive in the neighbourhood of a cell that exists,
         *                   then it survives.
         * 3. Otherwise, the cell dies.
         *
         * MAZECTRIC has rule B3/S1234 and seems to do better when the mazes are reversed
         * MAZE      has rule B3/S12345 and seems to do better when the mazes are NOT reversed
         * VOTE45    has rule B4678/S35678 and is atrocious, seeming to fill up the space entirely
         * VOTE      has rule B5678/S45678 and generates cavernous, toroidal mazes
         * B2S123    has rule B2/S123 and generates actually quite decent mazes.
         */
        enum Algorithm {
            MAZECTRIC,
            MAZE,
            VOTE45,
            VOTE,
            B2S123,
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
         * numGenerations indicates the maximum number of generations after random seeding that the automaton should
         *    produce. The default value is 10000. This will seldom be achieved.
         *
         *  stabilitySize is the number of previous generations we store in memory to which co compare the current
         *     generation. If the current generation is equivalent to one of the stored generations, we terminate.
         *     The default value is 5.
         *
         * neighbourhoodCounter determines how to count the surrounding neighbours of a cell.
         *    The default is using the Moore neighbourhood.
         *
         * determineBehaviour determines the behaviour of each cell with regards to the previous generation.
         *    The default is B2S123.
         *
         * The neighbourhoodCounter setting can be manually configured or configured from one of the NeighbourhoodType
         * values, MOORE or VON_NEUMANN_EXTENDED, by using the static fromNeighbourhoodType algorithm.
         *
         * The determineBehaviour setting can be manually configured or configured from one of the Algorithm values,
         * MAZECETRIC, MAZE, VOTE45, VOTE, or B2S123 by using the static fromAlgorithm method.
         */
        struct settings {
            double probability = 0.5;
            int numGenerations = 10000;
            int stabilitySize = 10;
            NeighbourCounter neighbourCounter = fromNeighbourhoodType(MOORE);
            DetermineBehaviour determineBehaviour = fromAlgorithm(B2S123);
        };

        CellularAutomatonThickMazeGenerator(int w, int h, settings &s);
        ~CellularAutomatonThickMazeGenerator() final = default;

        const ThickMaze generate() final;
    private:
        /// The settings for the cellular automaton.
        settings st;
    };
};
