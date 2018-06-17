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
    inline const int calculateNumWalls(const int w, const int h) {
        return (w - 1) * h + w * (h - 1);
    }

    /// Calculates the number of possible internal (non-boundary) walls in a maze of the specified dimensions.
    inline const int calculateNumWalls(const types::Dimensions2D &d) {
        return calculateNumWalls(d.getWidth(), d.getHeight());
    }
}
