/**
 * ThickMazeGeneratorByHomomorphism.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <maze/MazeGenerator.h>
#include "ThickMazeGenerator.h"

namespace spelunker::thickmaze {
    /**
     * This class takes a generator for a regular Maze and, via the homomorphism
     * from Maze to ThickMaze, wraps it and makes it into a generator for ThickMaze.
     * Note that the object takes ownership of the pointer passed in, and deletes it
     * when this object is deleted.
     */
    class ThickMazeGeneratorByHomomorphism final : public ThickMazeGenerator {
    public:
        /**
         * The constructor takes a MazeGenerator, and then, since the homomorphism
         * from a Maze of size w by h is to a ThickMaze of size 2w-1 by 2h-1, it
         * sets these parameters as such.
         * @param mg the MazeGenerator to wrap
         */
        ThickMazeGeneratorByHomomorphism(const maze::MazeGenerator *mg);
        ~ThickMazeGeneratorByHomomorphism() override;

        const ThickMaze generate() const noexcept override;

    private:
        const maze::MazeGenerator *mazeGenerator;
    };
}
