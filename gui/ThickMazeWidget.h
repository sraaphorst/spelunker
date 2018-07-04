/**
 * ThickMazeWidget.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <QWidget>

#include <tuple>

#include <thickmaze/ThickMaze.h>

namespace spelunker::gui {
    class ThickMazeWidget final : public QWidget {
    public:
        ThickMazeWidget(const thickmaze::ThickMaze &tm, QWidget *parent =  nullptr);

//        void resizeWithBorder(int w, int h);

    protected:
        void paintEvent(QPaintEvent *e) override;

    private:
        void doPainting();

//        std::pair<qreal, qreal> calculateCellSize(int mw = QWidget::width(),
//                                                  int mh = QWidget::height());

        thickmaze::ThickMaze maze;

        static auto const FLOOR_COLOUR1 = Qt::gray;
        static auto const FLOOR_COLOUR2 = Qt::lightGray;
        static auto const WALL_COLOUR  = Qt::black;
        static auto constexpr WALL_WIDTH = 2;
    };
}