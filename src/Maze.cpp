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
            initializeEmptyLayout(true);
            checkPositions();
        }

        void Maze::setLayout(const Layout &_layout) {
            layout = _layout;
            checkPositions();
        }

        void Maze::initializeEmptyLayout(bool walls) {
            for (auto r: layout) {
                for (auto c : r)
                    c.clear();
                r.clear();
            }
            layout.clear();

            // Set width x height of appropriate wall type.
            Cell cell(4, walls);
            Row row(width, cell);
            layout = Layout(height, row);
        }

        void Maze::checkPositions() {
            checkPosition(start);
            for (auto p: endingPositions)
                checkPosition(p);
        }

        void Maze::checkPosition(const vorpal::maze::Position &p) {
            if (p.first < 0 || p.first > width || p.second < 0 || p.second >= height)
                throw OutOfBoundsPosition(p);
        }
    }
}