/**
 * MazeWidget.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include "MazeWidget.h"

#include "../src/maze/Maze.h"
#include "../src/maze/DFSMazeGenerator.h"

#include <QApplication>
#include <QPen>
#include <QWidget>
#include <QPainter>
#include <QColor>

namespace spelunker::gui {
    MazeWidget::MazeWidget(const maze::Maze &m, QWidget *parent)
    : QWidget{parent}, maze{m} {}

    void MazeWidget::paintEvent(QPaintEvent *e) {

        Q_UNUSED(e)

        doPainting();
    }

    void MazeWidget::resizeWithBorder(int w, int h) {
        resize(w + 2 * BORDER_WIDTH, h + 2 * BORDER_WIDTH);
    }

    void MazeWidget::doPainting() {
        auto w = width();
        auto h = height();

        auto mw = maze.getWidth();
        auto mh = maze.getHeight();

        // Determine the size per cell at the current resolution.
        auto cellW = (1.0 * w - 2 * BORDER_WIDTH) / mw;
        auto cellH = (1.0 * h - 2 * BORDER_WIDTH) / mh;

        const QRect fullRect{0, 0, w, h};

        QPainter painter { this };
        //painter.setRenderHint(QPainter::HighQualityAntialiasing);
        painter.fillRect(fullRect, FLOOR_COLOUR);

        QPen pen{WALL_COLOUR};
        pen.setWidth(BORDER_WIDTH);
        painter.setPen(pen);
        painter.drawRect(fullRect);
        pen.setWidth(WALL_WIDTH);

        // Iterate over the cells.
        auto ypos = 1.0 * BORDER_WIDTH;
        for (auto y=0; y < mh; ++y) {
            auto xpos = 1.0 * BORDER_WIDTH;
            for (auto x=0; x < mw; ++x) {
                // Determine the walls.
                const auto UPPER_LEFT  = QPointF{xpos, ypos};
                const auto UPPER_RIGHT = QPointF{xpos + cellW, ypos};
                const auto LOWER_LEFT  = QPointF{xpos, ypos + cellH};
                const auto LOWER_RIGHT = QPointF{xpos + cellW, ypos + cellH};

                if (maze.wall(x, y, types::NORTH))
                    painter.drawLine(UPPER_LEFT, UPPER_RIGHT);
                if (maze.wall(x, y, types::EAST))
                    painter.drawLine(UPPER_RIGHT, LOWER_RIGHT);
                if (maze.wall(x, y, types::SOUTH))
                    painter.drawLine(LOWER_LEFT, LOWER_RIGHT);
                if (maze.wall(x, y, types::WEST))
                    painter.drawLine(UPPER_LEFT, LOWER_LEFT);

                xpos += cellW;
            }

            ypos += cellH;
        }
    }
}

#if MAZE_TYPE == 1
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
#endif