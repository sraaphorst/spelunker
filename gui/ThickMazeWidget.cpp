/**
 * ThickMazeWidget.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include "ThickMazeWidget.h"

#include "../src/maze/Maze.h"
#include "../src/maze/MazeTypeclasses.h"
#include "../src/maze/HuntAndKillMazeGenerator.h"
#include "../src/typeclasses/Homomorphism.h"
#include "../src/thickmaze/ThickMaze.h"
#include "../src/thickmaze/ThickMazeAttributes.h"
#include "MazeWidget.h"

#include <tuple>

#include <QApplication>
#include <QPen>
#include <QWidget>
#include <QPainter>
#include <QColor>

namespace spelunker::gui {
    ThickMazeWidget::ThickMazeWidget(const spelunker::thickmaze::ThickMaze &tm, QWidget *parent)
        : QWidget{parent}, maze{tm} {}

    void ThickMazeWidget::paintEvent(QPaintEvent *e) {

        Q_UNUSED(e)

        doPainting();
    }

//    void ThickMazeWidget::resizeWithBorder(int w, int h) {
//        // Calculate the size of each cell to set the border size.
//        const auto [x,y] = calculateCellSize(w, h);
//        resize(w + 2 * static_cast<int>(x), h + 2 * static_cast<int>(y));
//    }
//
//    std::pair<qreal, qreal> ThickMazeWidget::calculateCellSize(int w, int h) {
//        const auto mw = maze.getWidth();
//        const auto mh = maze.getHeight();
//
//        // Determine the size per cell at the current resolution.
//        const auto cellW = (1.0 * w) / mw;
//        const auto cellH = (1.0 * h) / mh;
//
//        return std::make_pair(cellW, cellH);
//    }

    void ThickMazeWidget::doPainting() {
        auto w = width();
        auto h = height();

        auto mw = maze.getWidth();
        auto mh = maze.getHeight();

        // Determine the size per cell at the current resolution.
        // Add two squares for the border.
        auto cellW = (1.0 * w) / (mw + 2);
        auto cellH = (1.0 * h) / (mh + 2);

        const QRect fullRect{0, 0, w, h};

        QPainter painter { this };
        painter.fillRect(fullRect, FLOOR_COLOUR);

        QPen pen{QBrush{Qt::SolidPattern}, WALL_COLOUR};
        painter.setPen(pen);

        // Iterate over the cells, including the border.
        auto ypos = 0.0;
        for (auto y = -1; y <= mh; ++y) {
            auto xpos = 0.0;
            for (auto x = -1; x <= mw; ++x) {
                // If this is a wall, draw it.
                if (x == -1 || x == mw || y == -1 || y == mh || maze.cellIs(x, y) == thickmaze::WALL)
                    painter.fillRect(QRectF{xpos, ypos, cellW, cellH}, WALL_COLOUR);

                xpos += cellW;
            }

            ypos += cellH;
        }
    }
}

#if MAZE_TYPE == 2
using namespace spelunker;
int main(int argc, char *argv[]) {
    QApplication app{argc, argv};

    // We will use a homomorphism to generate a ThickMaze. Leave room the the border.
    auto constexpr width  = 48;//24;
    auto constexpr height = 38;//19;
    auto constexpr cellSize = 10;

    maze::HuntAndKillMazeGenerator gen(width, height);
    maze::Maze m = gen.generate();
    thickmaze::ThickMaze tm = typeclasses::Homomorphism<maze::Maze, thickmaze::ThickMaze>::morph(m);

    gui::MazeWidget mwindow(m);
    mwindow.setWindowTitle("Maze");
    mwindow.resizeWithBorder(width * cellSize, height * cellSize);
    mwindow.setVisible(true);

    gui::ThickMazeWidget window{tm};
    window.setWindowTitle("ThickMaze");
    window.resize((2*width+2) * cellSize, (2*height+2) * cellSize);
    window.setVisible(true);

    return app.exec();
}
#endif