/**
 * RandomizedDFSMazeGenerator.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * A maze generator using the
 * <a href="https://en.wikipedia.org/wiki/Maze_generation_algorithm#Depth-first_search">
 * Depth-First Search algorithm.</a>
 */

#ifndef SPELUNKER_RANDOMIZEDDFSMAZEGENERATOR_H
#define SPELUNKER_RANDOMIZEDDFSMAZEGENERATOR_H

#include "MazeAttributes.h"
#include "MazeGenerator.h"

namespace vorpal::maze {
    class Maze;
    
    /// A @see{MazeGenerator} using the randomized DFS method.
    class RandomizedDFSMazeGenerator final : public MazeGenerator {
    public:
        RandomizedDFSMazeGenerator(int w, int h);
        virtual ~RandomizedDFSMazeGenerator() = default;

        const Maze generate() override;

    private:
        using CellRowIndicator = std::vector<bool>;
        using CellIndicator = std::vector<CellRowIndicator>;
        using Neighbours = std::vector<types::Position>;

        const Neighbours unvisitedNeighbours(const types::Cell &c, const CellIndicator &ci) const;
    };
};

#endif //SPELUNKER_RANDOMIZEDDFSMAZEGENERATOR_H
