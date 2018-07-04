/**
 * ThickMaze.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * ThickMaze represents a maze where the walls have thickness, i.e. take up a grid square.
 */

#pragma once

#include <iostream>

#include <types/AbstractMaze.h>
#include <types/BraidableMaze.h>
#include <types/Dimensions2D.h>
#include <types/ReversibleMaze.h>
#include <types/Transformation.h>
#include <types/TransformableMaze.h>

#include "ThickMazeAttributes.h"

namespace spelunker::thickmaze {
    /**
     * Whereas @see{Maze} represents a maze where the walls are zero thickness, ThickMaze represents a maze
     * where walls take up a grid space.
     *
     * There is a monomorphism from:
     * 1. Mazes of width w and height h to
     * 2. ThickMazes of width 2w-1 and height 2h-1.
     *
     * Thus, all of the @see{MazeGenerator}s can be used to make a subset of ThickMazes, i.e. ThickMazes of
     * odd width and height where walls are all contiguous cells of odd length >= 3. The mapping to this subset
     * is surjective, and thus Mazes and ThickMazes with this property are isomorphic.
     */
    class ThickMaze : public types::AbstractMaze,
                      public types::ReversibleMaze<ThickMaze>,
                      public types::BraidableMaze<ThickMaze>,
                      public types::TransformableMaze<ThickMaze> {
    public:
        /**
         * Create a ThickMaze with the given dimensions, start position, goal positions,
         * and contents.
         * @param d the dimensions of the maze, minus boundary cells
         * @param start an optional starting position
         * @param goals a collection of the goal positions
         * @param c the contents of the maze, minus boundary walls
         */
        ThickMaze(const types::Dimensions2D &d,
                  const types::PossibleCell &start,
                  const types::CellCollection &goals,
                  const CellContents &c);

        /**
         * Create a ThickMaze with the given width, height, and contents.
         * @param d the dimensions of the maze, minus boundary walls
         * @param c the contents of the maze, minus boundary walls
         */
        ThickMaze(const types::Dimensions2D &d, const CellContents &c);

        /**
         * Create a ThickMaze with the given dimensions, start position, goal positions,
         * and contents.
         * @param w the width of the maze, minus boundary walls
         * @param h the height of the maze, minus boundary walls
         * @param start an optional starting position
         * @param goals a collection of the goal positions
         * @param c the contents of the maze, minus boundary walls
         */
        ThickMaze(int w, int h,
                  const types::PossibleCell &start,
                  const types::CellCollection &goals,
                  const CellContents &c);

        /**
         * Create a ThickMaze with the given width, height, and contents.
         * This class is effectively immutable.
         * @param w the width of the maze, minus boundary walls
         * @param h the height of the maze, minus boundary walls
         * @param c the contents of the maze, minus boundary walls
         */
        ThickMaze(int w, int h, const CellContents &c);

        ~ThickMaze() override = default;

        /// Determine if two mazes are equal.
        bool operator==(const ThickMaze &other) const noexcept;

        /// Determine if two mazes are not equal.
        bool operator!=(const ThickMaze &other) const noexcept {
            return !(*this == other);
        }

        bool cellInBounds(const types::Cell &c) const noexcept override {
            const auto [x, y] = c;
            return getDimensions().cellInBounds(c) && contents[x][y] == CellType::FLOOR;
        }

        const CellType cellIs(int x, int y) const;

        const CellType cellIs(const types::Cell &c) const;

        /// Determine the number of walls a cell has.
        int numCellWalls(const types::Cell &c) const override;

        /// Apply a given transformation to this maze.
        const ThickMaze applyTransformation(types::Transformation t) const override;

        /**
         * This algorithm swaps walls and floors, not including the border wall. It is useful for some
         * cellular automata algorithms, which are sometimes more wall-connected than floor-connected.
         * @return the reverse of the original maze, with walls and floors swapper.
         */
        const ThickMaze reverse() const noexcept override;

        /// Make a ThickMaze into a braid maze, clearing dead ends with the given probability.
        /**
         * A braid maze is a maze with no dead ends: it will necessarily contain loops, and thus not be perfect.
         *
         * This method takes the current maze, and begins by creating a list of all the dead ends.
         * The list is then shuffled, and one-by-one, the algorithm considers each dead end cell and does the following:
         * 1. Determine if it is still indeed a dead end cell, as one of its walls may have already been removed.
         * 2. Use probability to determine if we apply braiding to this cell.
         * 3. Amongst the one-to-three neighbouring wall cells of this cell, pick one of the neighbours that
         *    eliminates more dead ends than any other. If multiple cells with the same count exist, pick randomly.
         * 4. Delete the wall cell.
         *
         * The reason for step (3) is because this allows us to minimize wall reduction, attempting to possibly
         * fix two dead ends at the same time (this cell, and the neighbour cell).
         *
         * @param probability the probability of fixing a given dead end
         * @return a new maze with walls removed to decrease the number of dead ends
         */
        const ThickMaze braid(double probability) const noexcept override;

        static ThickMaze load(std::istream &s);
        void save(std::ostream &s) const;

        const types::CellCollection neighbours(const types::Cell &c) const override;

    private:
        /// Determine the number of walls a cell has for an instance of Contents.
        /**
         * Determine the number of walls a cell has for an instance of Contents.
         * We allow Contents to be supplied for operations like {@see braid}, where we want to
         * be working on an intermediate new CellContents.
         * @param c the cell of interest
         * @param cc the CellContents to use in counting
         * @return the number of cell walls of c in cc
         */
        int numCellWallsInContents(const types::Cell &c, const CellContents &cc) const;

        /// Empty constructor for serialization.
        ThickMaze() = default;

        friend class boost::serialization::access;

        template<typename Archive>
        void serialize(Archive &ar, unsigned int version);

        const CellContents contents;
    };
}

BOOST_CLASS_VERSION(spelunker::thickmaze::ThickMaze, 1)
