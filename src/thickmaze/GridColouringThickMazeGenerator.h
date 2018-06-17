/**
 * GridColouringThickMazeGenerator.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * The general idea for this algorithm is available here:
 * https://www.gamasutra.com/blogs/HermanTulleken/20161005/282629/Algorithms_for_making_more_interesting_mazes.php
 */

#pragma once

#include <optional>
#include <tuple>
#include <vector>

#include <types/CommonMazeAttributes.h>
#include <types/Dimensions2D.h>

#include "GridColouring.h"
#include "ThickMazeAttributes.h"
#include "ThickMazeGenerator.h"

namespace spelunker::thickmaze {
    class ThickMaze;

    class GridColouringThickMazeGenerator final : ThickMazeGenerator {
    public:
        GridColouringThickMazeGenerator(const types::Dimensions2D &d,
                                        const GridColouring &gc, const GridColouring::CandidateConfiguration &cfg);
        GridColouringThickMazeGenerator(int w, int h,
                                        const GridColouring &gc, const GridColouring::CandidateConfiguration &cfg);
        ~GridColouringThickMazeGenerator() = default;

        const ThickMaze generate() const noexcept final;

    private:
        /// An aggregate wall, consisting of multiple positions.
        using AggregateWall = types::CellCollection;

        /// A collection of aggregate walls.
        using AggregateWallCollection = std::vector<AggregateWall>;

        /**
         * Given a position and some offsets, calculate the concrete coordinates. If these fall out of bounds
         * at any point, we return nothing.
         * @param c the cell from which to start
         * @param offsets the offsets to apply to the positions
         * @return the concrete positions if they exist, and nothing otherwise
         */
        std::optional<AggregateWall> offsetToAggregate(const types::Cell &c, const GridColouring::Offsets &offsets) const;

        /**
         * Given an aggregate wall, find the one or two rooms that it connects.
         * @param wall the wall, with fixed coordinates
         * @return a list of length one or two
         */
        types::CellCollection adjacentRooms(const AggregateWall &wall) const noexcept;

        /**
         * Given a cell c and the map of offsets
         * @param c the cell
         * @param offsetMap the
         * @return
         */
        AggregateWallCollection adjacentWalls(const types::Cell &c, const GridColouring::OffsetMap &offsetMap) const;



        const GridColouring gridColouring;
        const GridColouring::CandidateConfiguration configuration;
    };
}



