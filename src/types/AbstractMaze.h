/**
 * AbstractMaze.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * The abstract superclass of all mazes.
 */

#pragma once

#include "CommonMazeAttributes.h"
#include "Dimensions2D.h"
#include "Symmetry.h"

namespace spelunker::types {
    /**
     * An abstract superclass of all maze types, which manages information like the:
     * 1. width and height of the maze;
     * 2. the (possible) starting position of the maze
     * 3. the ending position(s) (can be empty) of the maze.
     */
    template<typename T>
    class AbstractMaze {
    protected:
        AbstractMaze(const Dimensions2D &d, const PossibleCell &startPos, const CellCollection &endPos);
        AbstractMaze(const Dimensions2D &d);
        AbstractMaze(int w, int h, const PossibleCell &startPos, const CellCollection &endPos);
        AbstractMaze(int w, int h);

        virtual ~AbstractMaze() = default;

    public:
        inline const Dimensions2D &getDimensions() const noexcept {
            return dimensions;
        }

        inline const int getWidth() const noexcept {
            return dimensions.getWidth();
        }

        inline const int getHeight() const noexcept {
            return dimensions.getHeight();
        }

        inline const PossibleCell getStartingCell() const noexcept {
            return startCell;
        }

        inline const CellCollection getGoalCells() const noexcept {
            return goalCells;
        }

        /// We make this virtual since the start position may not be valid in some cases, e.g. a wall in a ThickMaze.
        virtual void setStartingCell(const PossibleCell &startPos);

        /// We make this virtual since the goal positions may not be valid in some cases, e.g. a wall in a ThickMaze.
        virtual inline void setGoalCells(const CellCollection &cells);

        /**
         * Given a cell, check to see if the cell is in the dimensions.
         * If not, an OutOfBounds exception is thrown.
         * @param c the cell
         * @throws OutOfBoundsException
         */
        virtual void checkCell(const Cell &c) const;

        /**
         * Given a set of coordinates, check to see if they are in the dimensions.
         * If not, an OutOfBounds exception is thrown.
         * @param x the x coordinate
         * @param y the y coordinate
         * @throws OutOfBoundsException
         */
        virtual void checkCell(int x, int y) const;

        /**
         * Goven a set of coordinates, check to see if they are in the dimensions.
         * @param c the cell
         * @return true if the cell is in bounds, and false otherwise
         */
        virtual bool cellInBounds(const Cell &c) const noexcept;

        /**
         * Given a cell, check to see if the cell is in the dimensions.
         * @param x the x coodinate
         * @param y the y coordinate
         * @return true if the cell is in bounds, and false otherwise
         */
         virtual bool cellInBounds(int x, int y) const noexcept;

         /**
          * Apply a symmetry to this maze to get a new one.
          * Reflections in the diagonals can only be done in square mazes.
          * @param s the symmetry to apply
          * @return the transformed maze
          * @throws IllegalGroupOperation if the symmetry cannot be applied
          */
         virtual const T applySymmetry(Symmetry s) const = 0;

         /**
          * A braided maze is a maze with no dead ends.
          * Any maze can be made into a braided maze simply by removing one wall from each dead end.
          *
          * @return a braided or semi-braided maze
          */
         virtual const T braid() const noexcept  = 0;

    private:
        const Dimensions2D dimensions;
        PossibleCell startCell;
        CellCollection goalCells;
    };
}
