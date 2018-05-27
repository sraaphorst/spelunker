/**
 * ThickMazeGenerator.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Superclass of algorithms unique to @see{ThickMaze}s.
 */

#ifndef SPELUNKER_THICKMAZEGENERATOR_H
#define SPELUNKER_THICKMAZEGENERATOR_H

namespace spelunker::thickmaze {
    class ThickMaze;

    class ThickMazeGenerator {
    public:
        ThickMazeGenerator(int w, int h);
        virtual ~ThickMazeGenerator() = default;

        virtual const ThickMaze generate() = 0;

    protected:
        const int width;
        const int height;

        /// Create an empty set of contents (all FLOOR).
        inline types::CellContents createEmptyContents() {
            types::CellContents contents;
            contents.resize(width);
            for (auto i=0; i < width; ++i)
                contents[i].resize(height);
            return contents;
        }
    };
};

#endif //SPELUNKER_THICKMAZEGENERATOR_H
