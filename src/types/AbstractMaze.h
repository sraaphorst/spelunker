/**
 * AbstractMaze.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * The abstract superclass of all mazes.
 */

#pragma once

#include <queue>
#include <set>
#include <vector>

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
        AbstractMaze(const Dimensions2D &d, const PossibleCell &startPos, const CellCollection &endPos)
                : dimensions{d}, startCell{startPos}, goalCells{endPos} {
            if (d.getWidth() < 1 || d.getHeight() < 1)
                throw types::IllegalDimensions(d.getWidth(), d.getHeight());
            if (startCell)
                checkCell(*startCell);
            for (const auto g: goalCells)
                checkCell(g);
        }

        AbstractMaze(const Dimensions2D &d)
                : AbstractMaze{d, {}, CellCollection()} {}

        AbstractMaze(int w, int h, const PossibleCell &startPos, const CellCollection &endPos)
                : AbstractMaze{Dimensions2D{w, h}, startPos, endPos} {}

        AbstractMaze(int w, int h)
                : AbstractMaze{Dimensions2D{w, h}, {}, CellCollection()} {}

        virtual ~AbstractMaze() = default;

        bool operator==(const AbstractMaze<T> &other) const noexcept {
            return dimensions == other.dimensions &&
                   startCell == other.startCell &&
                   goalCells == other.goalCells;
        }

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
        virtual void setStartingCell(const PossibleCell &startPos) noexcept {
            if (startPos)
                checkCell(*startPos);
            startCell = startPos;
        }

        /// We make this virtual since the goal positions may not be valid in some cases, e.g. a wall in a ThickMaze.
        virtual void setGoalCells(const CellCollection &cells) noexcept {
            for (const auto c: cells)
                checkCell(c);
            goalCells = cells;
        }

        /**
         * Given a cell, check to see if the cell is in the dimensions.
         * If not, an OutOfBounds exception is thrown.
         * @param c the cell
         * @throws OutOfBoundsException
         */
        void checkCell(const Cell &c) const {
            dimensions.checkCell(c);
        }

        /**
         * Given a set of coordinates, check to see if they are in the dimensions.
         * If not, an OutOfBounds exception is thrown.
         * This is also the case if a cell has no outedges, which should be
         * the case of a wall or other out-of-bounds area.
         * @param x the x coordinate
         * @param y the y coordinate
         * @throws OutOfBoundsException
         */
        void checkCell(int x, int y) const {
            checkCell(types::cell(x, y));
        }

        /**
         * Given a set of coordinates, check to see if they are in the dimensions.
         * A cell with four walls, which would be the case for wall cells in ThickMazes, this returns false.
         * @param c the cell
         * @return true if the cell is in bounds, and false otherwise
         */
        bool cellInBounds(const Cell &c) const noexcept {
            return dimensions.cellInBounds(c) && numCellWalls(c) < 4;
        }

        /**
         * Given a cell, check to see if the cell is in the dimensions.
         * For mazes with out of bounds zones, such as ThickMaze, this will
         * return false if a wall.
         * @param x the x coodinate
         * @param y the y coordinate
         * @return true if the cell is in bounds, and false otherwise
         */
        bool cellInBounds(int x, int y) const noexcept {
            return cellInBounds(cell(x, y));
        }

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
         * @param probability the probability of eliminating a dead end
         * @return a braided or semi-braided maze
         */
        virtual const T braid(double probability) const noexcept = 0;

        /**
         * A braided maze is a maze with no dead ends.
         * Any maze can be made into a braided maze simply by removing one wall from each dead end.
         *
         * @param probability the probability of eliminating a dead end
         * @return a braided or semi-braided maze
         */
        const T braidAll() const noexcept {
            return braid(1.0);
        };

        /// Determine the number of walls a cell has.
        virtual int numCellWalls(const types::Cell &c) const = 0;

        /// Find the dead ends for this maze.
        /**
         * Find a collection of all the dead ends for this maze.
         * A cell is considered a "dead end" if it has exactly three walls.
         * @return a collection of the dead end cells
         */
        const types::CellCollection findDeadEnds() const noexcept {
            types::CellCollection deadends;
            const auto[width, height] = dimensions.values();
            for (auto y = 0; y < height; ++y) {
                for (auto x = 0; x < width; ++x) {
                    const auto c = types::cell(x, y);
                    if (numCellWalls(c) == 3)
                        deadends.emplace_back(c);
                }
            }
            return deadends;
        }

        /**
         * Empty constructor for serialization.
         */
        AbstractMaze()
                : dimensions{Dimensions2D{1, 1}}, startCell{}, goalCells{CellCollection{}} {}

        /// Find the neighbours of a given cell.
        /**
         * Given a cell c, find its neighbours in the maze.
         * This will allow us to implement BFS here in AbstractMaze.
         * @param c the cell in question
         * @return a list of its direct neighbours
         */
        virtual const types::CellCollection neighbours(const types::Cell &c) const = 0;

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
         * Subclasses will have to check that the given start cell is in bounds.
         * @param start the starting cell
         * @return the data collected during the BFS
         */
        const BFSResults performBFSFrom(const types::Cell &start) const {
            checkCell(start);

            // Keep track of the cells to which we're connected.
            CellCollection connectedCells;

            // Keep track of the distances of the cells.
            // start is the only cell at position zero so record it.
            CellDistances distances{1};

            // Prepare the queue for BFS. The cell info comprises a cell and its distance.
            using cellInfo = std::pair<const types::Cell, const int>;
            std::queue<cellInfo> cellQueue{};
            cellQueue.push(cellInfo{start, 0});

            // We also want to keep track of the cells visited.
            auto ci = initializeCellIndicator(getDimensions(), false);

            while (!cellQueue.empty()) {
                const auto[cell, dist] = cellQueue.front();
                const auto[cellx, celly] = cell;
                cellQueue.pop();

                // If this cell has already been visited, ignore.
                if (ci[cellx][celly])
                    continue;

                // Mark the cell visited, the distance and add the neighbours.
                ci[cellx][celly] = true;
                connectedCells.emplace_back(cell);
                if (distances.size() <= dist)
                    distances.resize(dist+1);
                distances[dist].emplace_back(cell);

                const auto nbrs = neighbours(cell);
                for (auto n: nbrs)
                    cellQueue.push(cellInfo{n, dist + 1});
            }

            return BFSResults{start, connectedCells, distances};
        }

        /**
         * This method looks through the cells of the maze, and returns those that are considered invalid, i.e.
         * those that have four walls.
         * @return a collection of the invalid cells
         */
        const CellCollection findInvalidCells() const noexcept {
            CellCollection cc;
            for (auto y = 0; y < getHeight(); ++y)
                for (auto x = 0; x < getWidth(); ++x)
                    if (!cellInBounds(x, y))
                        cc.emplace_back(cell(x, y));
            return cc;
        }

        const ConnectedComponents findConnectedComponents() const noexcept {
            ConnectedComponents cc;

            // We need to keep track of which cells we've visited.
            // Start by marking the out-of-bound cells as "visited", since we don't want to visit them.
            CellIndicator ci = initializeCellIndicator(dimensions);
            const auto invalidCells = findInvalidCells();
            for (const auto ic: invalidCells) {
                const auto [x, y] = ic;
                ci[x][y] = true;
            }

            for (auto y = 0; y < getHeight(); ++y)
                for (auto x = 0; x < getWidth(); ++x) {
                    if (ci[x][y]) continue;
                    const auto bfsResults = performBFSFrom(cell(x, y));
                    const auto comp = bfsResults.connectedCells;
                    for (const auto c: comp) {
                        const auto[cx, cy] = c;
                        ci[cx][cy] = true;
                    }
                    cc.emplace_back(comp);
                }
            return cc;
        }

    private:
        const Dimensions2D dimensions;
        PossibleCell startCell;
        CellCollection goalCells;

        template<typename Archive>
        void serialize(Archive &ar, const unsigned int version) {
            ar & const_cast<Dimensions2D &>(dimensions);
            ar & startCell;
            ar & goalCells;
        }

        friend class boost::serialization::access;
    };
}
