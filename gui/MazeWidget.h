/**
 * MazeWidget.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * A Qt widget for displaying mazes.
 */

#pragma once

#include "../src/maze/Maze.h"
#include <QWidget>

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

        static auto const FLOOR_COLOUR = Qt::gray;
        static auto const WALL_COLOUR  = Qt::black;
        static auto constexpr WALL_WIDTH = 2;
        static auto constexpr BORDER_WIDTH = 5;
    };
}