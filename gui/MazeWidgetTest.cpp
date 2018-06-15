/**
 * MazeWidgetTest.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <QApplication>

#include <maze/DFSMazeGenerator.h>
#include <maze/Maze.h>
#include "MazeWidget.h"

using namespace spelunker;
int main(int argc, char *argv[]) {
    QApplication app{argc, argv};

    auto constexpr width  = 50;
    auto constexpr height = 40;
    auto constexpr cellSize = 20;

    maze::DFSMazeGenerator gen{width, height};
    maze::Maze m = gen.generate();

    gui::MazeWidget window(m);
    window.setWindowTitle("Maze");
    window.resizeWithBorder(width * cellSize, height * cellSize);
    window.setVisible(true);

    return app.exec();
}