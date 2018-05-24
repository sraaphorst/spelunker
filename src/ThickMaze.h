/**
 * ThickMaze.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * ThickMaze represents a maze where the walls have thickness, i.e. take up a grid square.
 */

#ifndef SPELUNKER_THICKMAZE_H
#define SPELUNKER_THICKMAZE_H

#include <sstream>
#include <string>

#include "Show.h"
#include "ThickMazeAttributes.h"
#include "StringThickMazeRenderer.h"

namespace spelunker::thickmaze {
    /**
     * Whereas @see{Maze} represents a maze where the walls are zero thickness, ThickMaze represents a maze
     * where walls take up a grid space.
     *
     * There is a monomorphism (but not surjective) from:
     * 1. Mazes of width w and height h to
     * 2. ThickMazes of width 2w-1 and height 2h-1.
     *
     * Thus, all of the @see{MazeGenerator}s can be used to make a subset of ThickMazes, i.e. ThickMazes of
     * odd width and height where walls are all contiguous cells of odd length >= 3. The mapping to this subset
     * is surjective, and thus Mazes and ThickMazes with this property are isomorphic.
     */
    class ThickMaze final {
    public:
        /**
         * Create a ThickMaze with the given width, height, and contents.
         * This class is effectively immutable.
         * @param w the width of the maze, minus boundary walls
         * @param h the height of the maze, minus boundary walls
         * @param c the contents of the maze, minus boundary walls
         */
        ThickMaze(int w, int h, const types::CellContents &c);
        virtual ~ThickMaze() = default;

        inline const int getWidth() const noexcept { return width; }
        inline const int getHeight() const noexcept { return height; }

        const types::CellType cell(int x, int y) const;

    private:
        const int width;
        const int height;
        const types::CellContents contents;
    };
}

namespace spelunker::typeclasses {
    template<>
    struct Show<thickmaze::ThickMaze> {
        static std::string show(const thickmaze::ThickMaze &tm) {
            std::ostringstream out;
            thickmaze::StringThickMazeRenderer r(out);
            r.render(tm);
            return out.str();
        }
    };
}
#endif //SPELUNKER_THICKMAZE_H
