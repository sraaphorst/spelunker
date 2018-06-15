/**
 * ThickMazeWidget.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <QColor>
#include <QPainter>
#include <QPen>
#include <QWidget>

#include <tuple>

#include <thickmaze/ThickMaze.h>
#include "ThickMazeWidget.h"

namespace spelunker::gui {
    ThickMazeWidget::ThickMazeWidget(const spelunker::thickmaze::ThickMaze &tm, QWidget *parent)
        : QWidget{parent}, maze{tm} {}

    void ThickMazeWidget::paintEvent(QPaintEvent *e) {

        Q_UNUSED(e)

        doPainting();
    }

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

        const QPen pen{QBrush{Qt::SolidPattern}, WALL_COLOUR};
        painter.setPen(pen);

        // Iterate over the cells, including the border: hence [-1,mh] / [-1,mw] ranges.
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
