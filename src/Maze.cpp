/**
 * Maze.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <vector>
#include "Exceptions.h"
#include "MazeAttributes.h"
#include "Maze.h"

namespace vorpal {
    namespace maze {
        Maze::Maze(const int _width, const int _height, const Position _start, const PositionCollection _endingPositions)
        : width(_width), height(_height), start(_start), endingPositions(_endingPositions) {
            initializeEmptyLayout();
            checkPositions();
        }

        void Maze::setLayout(const Layout &_layout) {
            layout = _layout;
            checkPositions();
        }

        void Maze::initializeEmptyLayout() {
            for (auto l: layout) l.clear();
            layout.clear();

            // Set width x height of EMPTY.
            std::vector< MazeType > row(width, MazeType::EMPTY);
            layout = std::vector< std::vector< MazeType > > (height, row);
        }

        void Maze::checkPositions() {
            checkPosition(start);
            for (auto p: endingPositions)
                checkPosition(p);
        }

        void Maze::checkPosition(const vorpal::maze::Position &p) {
            if (p.first < 0 || p.first > width || p.second < 0 || p.second >= height)
                throw OutOfBoundsPosition(p);

            const MazeType mt = layout[p.first][p.second];
            if (layout[p.first][p.second] != MazeType::EMPTY)
                throw IllegalPositionType(p, mt);
        }
    }
}