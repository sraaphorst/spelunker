/**
 * RoomFinder.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * This allows you to collapse rooms down to one "pseudocell" for maze squashing.
 * Mazes that do not have their rooms collapsed will likely not work properly by the squasher.
 */

#pragma once

#include <map>
#include <vector>

#include <types/AbstractMaze.h>
#include <types/CommonMazeAttributes.h>

namespace spelunker::squashedmaze {
    /**
     * This class takes an AbstractMaze and tries to find a representation that collapses it down into "rooms".
     * The way this works is that each accessible cell is initialized with a unique room number, and we have
     * two collections:
     * 1. Map of Cell -> Room#, say C(c)
     * 2. Map of Room# -> CellCollection, say R(r)
     *
     * We iteratively traverse the maze as follows:
     *
     * For a cell c1 = (x,y), if c2 = (x+1,y), c3 = (x,y+1), and c4 = (x+1,y+1) all exist and have no walls between them,
     * and for any c_i, c_j, we have that C(c_i) != C(c_j), then for all c_i = c2, c3, c4:
     * 1. Add all cells in R(C(c_i)) to R(C(c1)), and for all cells c in R(C(c_i)), set R(c) = R(c1).
     * 2. Delete any empty rooms, i.e. R(r) = emptyset.
     *
     * We stop when we reach stability, i.e. M(c) and R(r) do not change.
     */
    class RoomFinder final {
    public:
        using RoomID = int;

        // We look up the room assignment of cells by a 2D array.
        using CellToRoomColumn = std::vector<RoomID>;
        using CellToRoom = std::vector<CellToRoomColumn>;
        using RoomContents = std::map<RoomID, types::CellCollection>;

        explicit RoomFinder(const types::AbstractMaze &maze);
        ~RoomFinder() = default;

        /// Get the map from cells to room IDs.
        inline const CellToRoom &getCellToRoom() const noexcept {
            return cellToRoom;
        }

        /// Get the map from room IDs to their contents.
        inline const RoomContents &getRoomContents() const noexcept {
            return roomContents;
        }

    private:
        CellToRoom cellToRoom;
        RoomContents roomContents;
    };
}

