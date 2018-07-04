/**
 * RoomFinder.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <algorithm>
#include <map>
#include <vector>

#include <types/AbstractMaze.h>
#include <types/CommonMazeAttributes.h>
#include "RoomFinder.h"

namespace spelunker::squashedmaze {

    RoomFinder::RoomFinder(const types::AbstractMaze &maze)
            : cellToRoom(maze.getWidth(), CellToRoomColumn(maze.getHeight(), -1)), roomContents{} {

        const auto width = maze.getWidth();
        const auto height = maze.getHeight();

        CellToRoom C(width, CellToRoomColumn(height, -1));
        RoomContents R;

        // Initialize the CellToRoom and RoomContents:
        RoomID r = 0;
        for (auto y = 0; y < height; ++y) {
            for (auto x = 0; x < width; ++x) {
                if (maze.cellInBounds(x, y)) {
                    C[x][y] = r;
                    R[r] = types::CellCollection{types::cell(x, y)};
                    ++r;
                }
            }
        }

        // Loop bounds: we are considering 2x2 blocks, so we don't iterate to the end.
        const int widthUpper = width - 1;
        const int heightUpper = height - 1;

        // Keep track of if we have changed or not, and quit when we haven't.
        bool changed = true;
        while (changed) {
            changed = false;

            for (auto y = 0; y < heightUpper; ++y) {
                for (auto x = 0; x < widthUpper; ++x) {
                    // The 2x2 grid must be in bounds.
                    if (!(maze.cellInBounds(x, y) && maze.cellInBounds(x + 1, y) && maze.cellInBounds(x, y + 1) &&
                          maze.cellInBounds(x + 1, y + 1)))
                        continue;

                    // If the cells are already in the same room, skip.
                    const int test_room_id = C[x][y];
                    if (C[x + 1][y] == test_room_id && C[x][y + 1] == test_room_id && C[x + 1][y + 1] == test_room_id)
                        continue;

                    // There must be no walls between the cells.
                    // This calculation will be more difficult.
                    // We set up the cells like this:
                    // C0 C1
                    // C3 C2
                    // and then can do the check using a loop with calculations mod 4, checking for C_i that
                    // C_{i+1 mmod 4} and C_{i-1 mod 4} are neighbours.
                    types::CellCollection candidates{types::cell(x, y), types::cell(x + 1, y),
                                                     types::cell(x + 1, y + 1), types::cell(x, y + 1)};

                    bool noWalls = true;
                    for (auto i = 0; i < 4; ++i) {
                        const auto nbrs = maze.neighbours(candidates[i]);
                        if ((std::find(nbrs.begin(), nbrs.end(), candidates[(i + 1) % 4]) == nbrs.end())
                            || (std::find(nbrs.begin(), nbrs.end(), candidates[(i + 4 - 1) % 4]) == nbrs.end())) {
                            noWalls = false;
                            break;
                        }
                    }
                    if (!noWalls)
                        continue;

                    // Get the smallest RoomID to be the room ID we will use. We do this to avoid cycling when
                    // a cell c belongs to multiple rooms. By always picking the lowest room ID, we will eventually
                    // reach stability instead of fluctuating between several room ID based on c.
                    std::vector<RoomID> allRoomIDs(4);
                    std::transform(candidates.begin(), candidates.end(), allRoomIDs.begin(),
                                   [&C](auto c) { return C[c.first][c.second]; });
                    const auto room_id = *std::min_element(allRoomIDs.begin(), allRoomIDs.end());

                    // We have all four cells connected and with no walls. We must join them together.
                    auto &room = R.at(room_id);
                    for (const auto curr: candidates) {
                        const auto[cx, cy] = curr;
                        const auto curr_room_id = C[cx][cy];
                        auto &curr_room = R.at(curr_room_id);

                        // Add it to the new room.
                        room.push_back(curr);
                        C[cx][cy] = room_id;

                        // Remove it from its old room.
                        curr_room.erase(std::find(curr_room.begin(), curr_room.end(), curr));
                    }

                    changed = true;
                }
            }
        }


        // Now we re-process the data, throwing away what we don't need.
        // 1. Ultimately, we want to normalize the rooms to start at 0, and only contain rooms with > 1 elements.
        // 2. Furthermore, we want our cell-to-room lookup to only contain RoomIDs if the cell is valid and belongs
        //    to a non-trivial room.
        int nextRoom = 0;
        for (const auto &r: R) {
            const auto &[id, contents] = r;
            if (contents.size() <= 1) continue;

            roomContents[nextRoom] = contents;
            for (const auto c: contents) {
                const auto [cx, cy] = c;
                cellToRoom[cx][cy] = nextRoom;
            }
            ++nextRoom;
        }
    }
}