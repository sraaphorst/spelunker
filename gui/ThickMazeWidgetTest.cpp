/**
 * ThickMazeWidgetTest.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <QApplication>
#include <QDesktopWidget>

#include <maze/HuntAndKillMazeGenerator.h>
#include <maze/Maze.h>
#include <maze/MazeTypeclasses.h>
#include <thickmaze/ThickMaze.h>
#include <thickmaze/ThickMazeAttributes.h>
#include <typeclasses/Homomorphism.h>

#include "ThickMazeWidget.h"
#include "MazeWidget.h"

using namespace spelunker;
int main(int argc, char *argv[]) {
    QApplication app{argc, argv};

    // We will use a homomorphism to generate a ThickMaze. Leave room the the border.
    auto constexpr mazeWidth  = 10;
    auto constexpr mazeHeight = 10;
    auto constexpr cellSize = 10;
    auto constexpr windowSeparation = 20;

    maze::HuntAndKillMazeGenerator gen(mazeWidth, mazeHeight);
    maze::Maze m = gen.generate();
    thickmaze::ThickMaze tm = typeclasses::Homomorphism<maze::Maze, thickmaze::ThickMaze>::morph(m);

    auto d = app.desktop();
    auto dwidth  = d->width();
    auto dheight = d->height();

    gui::ThickMazeWidget window{tm};
    window.setWindowTitle("ThickMaze");
    window.resize((2*mazeWidth+2) * cellSize, (2*mazeHeight+2) * cellSize);

    gui::MazeWidget mwindow(m);
    mwindow.setWindowTitle("Maze");
    mwindow.resizeWithBorder(mazeWidth * cellSize, mazeHeight * cellSize);

    // Determine the coordinates of both windows so we can place them, side-by-side,
    // in the center of the screen.
    auto wx  = (dwidth - window.width() - mwindow.width() - windowSeparation) / 2;
    auto mwx = wx +  window.width() + windowSeparation;
    auto y = (dheight - window.height()) / 2;
    window.move(wx, y);
    mwindow.move(mwx, y);

    mwindow.setVisible(true);
    mwindow.raise();
    window.setVisible(true);
    window.raise();

    return app.exec();
}