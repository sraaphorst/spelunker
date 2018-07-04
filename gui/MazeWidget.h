/**
 * MazeWidget.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * A Qt widget for displaying mazes.
 */

#pragma once

#include <QWidget>

#include <maze/Maze.h>

namespace spelunker::gui {
    class MazeWidget final : public QWidget {
    public:
        MazeWidget(const maze::Maze &m, QWidget *parent = nullptr);

        void resizeWithBorder(int w, int h);

    protected:
        void paintEvent(QPaintEvent *e) override;

    private:
        void doPainting();

        maze::Maze maze;

        static auto const FLOOR_COLOUR1    = Qt::gray;
        static auto const FLOOR_COLOUR2    = Qt::lightGray;
        static auto const WALL_COLOUR      = Qt::black;
        static auto constexpr WALL_WIDTH   = 1;
        static auto constexpr BORDER_WIDTH = 3;
    };
}