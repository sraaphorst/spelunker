/**
 * ThickMazeGeneratorByHomomorphism.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <typeclasses/Homomorphism.h>
#include <maze/Maze.h>
#include <maze/MazeGenerator.h>
#include <maze/MazeTypeclasses.h>
#include "ThickMazeGenerator.h"

namespace spelunker::thickmaze {
    /**
     * This class takes a generator for a regular Maze and, via the homomorphism
     * from Maze to ThickMaze, wraps it and makes it into a generator for ThickMaze.
     * Note that the object takes ownership of the pointer passed in, and deletes it
     * when this object is deleted.
     */
    template<typename MG>
    class ThickMazeGeneratorByHomomorphism final : public ThickMazeGenerator {
    public:
        /**
         * The constructor takes a MazeGenerator, and then, since the homomorphism
         * from a Maze of size w by h is to a ThickMaze of size 2w-1 by 2h-1, it
         * sets these parameters as such.
         * @param mg the MazeGenerator to wrap
         */
        explicit ThickMazeGeneratorByHomomorphism(MG&& mg)
                : ThickMazeGenerator{types::Dimensions2D{2 * mg.getWidth() - 1, 2 * mg.getHeight() - 1}}, mazeGenerator{std::move(mg)} {}
        explicit ThickMazeGeneratorByHomomorphism(const MG&& mg)
                : ThickMazeGenerator{types::Dimensions2D{2 * mg.getWidth() - 1, 2 * mg.getHeight() - 1}}, mazeGenerator{std::move(mg)} {}
        explicit ThickMazeGeneratorByHomomorphism(const MG &mg)
                : ThickMazeGenerator{types::Dimensions2D{2 * mg.getWidth() - 1, 2 * mg.getHeight() - 1}}, mazeGenerator{mg} {}

        ~ThickMazeGeneratorByHomomorphism() override = default;

        const ThickMaze generate() const noexcept override {
            const auto m = mazeGenerator.generate();
            const ThickMaze tm = typeclasses::Homomorphism<maze::Maze, ThickMaze>::morph(m);
            return tm;
        }

    private:
        const MG mazeGenerator;
    };
}
