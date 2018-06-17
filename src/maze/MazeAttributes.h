/**
 * MazeAttributes.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Attributes specific to Mazes.
 */

#pragma once

#include <map>
#include <vector>

#include <types/CommonMazeAttributes.h>
#include <types/Dimensions2D.h>

namespace spelunker::maze {
    /// The ID for a wall. Walls are ranked to ints.
    using WallID = int;

    /// The wall incidence of a maze.
    /**
     * The incidence of walls in a maze, which is a map from the rank of the wall to a bool indicating
     * its existence. This, with Layout, determines the maze. We represent this by vector since we assume
     * a contiguous range of wall incidences.
     */
    using WallIncidence = std::vector<bool>;

    /// A collection of walls for processing.
    using WallCollection = std::vector<WallID>;

    /// Used to reverse wall ranking, i.e. a map that takes a wall rank and gives the two cells it separates.
    using UnrankWallMap = std::map<WallID, std::pair<types::Position, types::Position>>;

    /// Calculates the number of possible internal (non-boundary) walls in a maze of width w and height h.
    const int calculateNumWalls(int width, int height);

    /// Calculates the number of possible internal (non-boundary) walls in a maze of the specified dimensions.
    const int calculateNumWalls(const types::Dimensions2D &d) noexcept;

    /// Create an "empty layout", either full of walls, or without any walls (other than the boundary walls).
    /**
     * Initialize the layout of the maze to the "empty" layout.
     * This consists of the wall incidences all being set to true or false.
     * The boundary walls are always maintained.
     * @param w the width of the layout
     * @param walls indicates whether the maze should be all walls or no walls (except the boundary wall).
     * @return an "empty" layout
     */
    WallIncidence createMazeLayout(int width, int height, bool walls = true);

    /// Create an "empty layout", either full of walls, or without any walls (other than the boundary walls).
    /**
     * Initialize the layout of the maze to the "empty" layout.
     * This consists of the wall incidences all being set to true or false.
     * The boundary walls are always maintained.
     * @param d the dimension of the layout
     * @param walls indicates whether the maze should be all walls or no walls (except the boundary wall).
     * @return an "empty" layout
     */
    WallIncidence createMazeLayout(const types::Dimensions2D & d, bool walls = true) noexcept;
}
