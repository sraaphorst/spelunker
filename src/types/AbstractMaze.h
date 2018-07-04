/**
 * AbstractMaze.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * The abstract superclass of all mazes.
 */

#pragma once

#include <climits>
#include <cmath>
#include <queue>
#include <set>
#include <vector>
#include <iostream>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/optional.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/serialization/version.hpp>
#include <boost/mpl/int.hpp>

#include "CommonMazeAttributes.h"
#include "Exceptions.h"
#include "Dimensions2D.h"
#include "Transformation.h"

namespace spelunker::types {
    /**
     * An abstract superclass of all maze types, which manages information like the:
     * 1. width and height of the maze;
     * 2. the (possible) starting position of the maze
     * 3. the ending position(s) (can be empty) of the maze.
     */
    class AbstractMaze {
    protected:
        AbstractMaze(const Dimensions2D &d, const PossibleCell &startPos, const CellCollection &endPos);
        AbstractMaze(const Dimensions2D &d);
        AbstractMaze(int w, int h, const PossibleCell &startPos, const CellCollection &endPos);
        AbstractMaze(int w, int h);

        virtual ~AbstractMaze() = default;

        /**
         * For two mazes to be equal, they must have the same start, goals, and dimensions.
         * Subclasses should extend this to add content checking to it.
         * @param other the other maze
         * @return true if they are equal, and false otherwise
         */
        bool operator==(const AbstractMaze &other) const noexcept;

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
        virtual void setStartingCell(const PossibleCell &startPos) noexcept;

        /// We make this virtual since the goal positions may not be valid in some cases, e.g. a wall in a ThickMaze.
        virtual void setGoalCells(const CellCollection &cells) noexcept;

        /**
         * Given a cell, check to see if the cell is in the dimensions.
         * If not, an OutOfBounds exception is thrown.
         * @param c the cell
         * @throws OutOfBoundsException
         */
        void checkCell(const Cell &c) const;

        /**
         * Given a set of coordinates, check to see if they are in the dimensions.
         * If not, an OutOfBounds exception is thrown.
         * This is also the case if a cell has no outedges, which should be
         * the case of a wall or other out-of-bounds area.
         * @param x the x coordinate
         * @param y the y coordinate
         * @throws OutOfBoundsException
         */
        void checkCell(int x, int y) const;

        /**
         * Given a set of coordinates, check to see if they are in the dimensions.
         * A cell with four walls, which would be the case for wall cells in ThickMazes, this returns false.
         * @param c the cell
         * @return true if the cell is in bounds, and false otherwise
         */
        virtual bool cellInBounds(const Cell &c) const noexcept;

        /**
         * Given a cell, check to see if the cell is in the dimensions.
         * For mazes with out of bounds zones, such as ThickMaze, this will
         * return false if a wall.
         * @param x the x coodinate
         * @param y the y coordinate
         * @return true if the cell is in bounds, and false otherwise
         */
        bool cellInBounds(int x, int y) const noexcept;

        /// Determine the number of walls a cell has.
        virtual int numCellWalls(const types::Cell &c) const = 0;

        /// Find the dead ends for this maze.
        /**
         * Find a collection of all the dead ends for this maze.
         * A cell is considered a "dead end" if it has exactly three walls.
         * @return a collection of the dead end cells
         */
        const types::CellCollection findDeadEnds() const noexcept;

        /// Find the junctions for this maze.
        /**
         * Find a collection of all the junctions for this maze.
         * A cell is considered a "junction" if it has exactly zero or one walls,
         * thus offering a choice of movement.
         * A junction with one wall is called a <em>T junction</em>.
         * A junction with zero walls is called a <em>+ junction</em>.
         * @return a collection of the junction cells
         */
         const types::CellCollection findJunctions() const noexcept;

         /// Count the number of carved walls for the maze.
         /**
          * Count the number of walls that the maze has carved out.
          * @return the number of carved walls
          */
          const int numCarvedWalls() const noexcept;

        /// Find the neighbours of a given cell.
        /**
         * Given a cell c, find its neighbours in the maze.
         * This will allow us to implement BFS here in AbstractMaze.
         * @param c the cell in question
         * @return a list of its direct neighbours
         */
        virtual const types::CellCollection neighbours(const types::Cell &c) const = 0;

        /// Find the neighbours of a group of cells.
        /**
         * Given a CellCollection, find its neighbours int he maze.
         *
         */
        const types::CellSet neighbours(const types::CellCollection &cc) const;

        /// Performs a BFS from the given input cell and returns the results.
        /**
         * This algorithm begins at the specified starting cell, and returns
         * the BFSResults information as described in
         * @see{CommonMazeAttrinbutes::BFSResults}. It will be the basis for:
         * 1. Finding the "best" start and end positon in a maze;
         * 2. Finding the connected components of a maze; and
         * 3. Being able to apply solutions to all maze types through
         *    `AbstractMaze`.
         *
         * It will also emit events so that the process of finding connected
         * components and the cells at various distances (perhaps by darkening
         * of the colour for the component as we get further away) can be
         * visually displayed through a UI.
         *
         * Unfortunately, as this is a template, we insantiate this long bit of code here.
         * Note that this algorithm is quite slow, so it should not be repeated many times.
         * @param start the starting cell
         * @return the data collected during the BFS
         */
        const BFSResults performBFSFrom(const types::Cell &start) const;

        /**
         * This method looks through the cells of the maze, and returns those that are considered invalid, i.e.
         * those that have four walls.
         * @return a collection of the invalid cells
         */
        const CellCollection findInvalidCells() const noexcept;

        /// Find the connected components of the maze.
        /**
         * Find the connected components of the Maze. Note that this does not include invalid cells, i.e.
         * the findInvalidCells method will return the final class in a partitioning of the cells along with
         * this method.
         * @return a collection of connected components
         */
        const ConnectedComponents findConnectedComponents() const noexcept;

        /**
         * Find the diameter of the graph. This consists of the longest distance between any pair
         * of points. To do so, we can find the shortest distance between any two vertices and
         * then take the maximum.
         *
         * Instead of using performBFSFrom, we implement an optimized algorithm that only stores
         * the necessary information instead of the information stored at every node in performBFSFrom.
         * This cuts the algorithm execution time down dramatically.
         *
         * I attempted a number of all-pairs shortest path algorithms here. A good source for them
         * can be found here:
         * http://jeffe.cs.illinois.edu/teaching/algorithms/notes/22-apsp.pdf
         * Regardless, this approach was much faster than any of them.
         *
         * @return a structure with the distance and a list of the pairs of cells at that distance
         */
        const FurthestCellResults findDiameter() const noexcept;

    protected:
        /**
         * Empty constructor for Boost.Serialization.
         */
        AbstractMaze();

        template<typename Archive>
        void serialize(Archive &ar, const unsigned int version) {
                ar & const_cast<Dimensions2D &>(dimensions);
                ar & startCell;
                ar & goalCells;
        }

    private:
        const Dimensions2D dimensions;
        PossibleCell startCell;
        CellCollection goalCells;

        /// The maximum number of cell walls. We may change this later, so isolate it here.
        static constexpr int NumWalls = 4;

        friend class boost::serialization::access;
    };
}

