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

    protected:
        void paintEvent(QPaintEvent *e) override;

    private:
        void doPainting();

        thickmaze::ThickMaze maze;

        static auto const FLOOR_COLOUR1 = Qt::gray;
        static auto const FLOOR_COLOUR2 = Qt::lightGray;
        static auto const WALL_COLOUR  = Qt::black;
    };
}