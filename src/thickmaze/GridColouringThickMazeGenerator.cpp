/**
 * GridColouringThickMazeGenerator.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

// We use Boost optional instead of STL optional for serialization purposes.
#include <boost/optional.hpp>

#include <algorithm>
#include <cassert>
#include <tuple>
#include <vector>

#include <types/CommonMazeAttributes.h>
#include <types/Dimensions2D.h>
#include <math/RNG.h>

#include "GridColouring.h"
#include "ThickMaze.h"
#include "ThickMazeAttributes.h"

#include "GridColouringThickMazeGenerator.h"

#ifdef DEBUG
#include <iostream>
using namespace std;
#endif

namespace spelunker::thickmaze {
    GridColouringThickMazeGenerator::GridColouringThickMazeGenerator(const types::Dimensions2D &d,
                                    const GridColouring &gc, const GridColouring::CandidateConfiguration &cfg)
        : ThickMazeGenerator{d}, gridColouring{gc}, configuration{cfg} {}

    GridColouringThickMazeGenerator::GridColouringThickMazeGenerator(int w, int h, const GridColouring &gc,
                                                                     const GridColouring::CandidateConfiguration &cfg)
        : GridColouringThickMazeGenerator{types::Dimensions2D{w, h}, gc, cfg} {}


    const ThickMaze GridColouringThickMazeGenerator::generate() const noexcept {
#ifdef DEBUG
        cout << "Using following colouring:" << endl;
        for (auto i=0; i < gridColouring.numRows(); ++i) {
            cout << "\t";
            for (auto j = 0; j < gridColouring.numCols(); ++j)
                cout << gridColouring.cellColour(j, i) << " ";
            cout << endl;
        }
        cout << "Room colour: " << configuration.roomColour << endl;
        cout << "Walls: ";
        for (const auto &xyz: configuration.walls) {
            cout << '[';
            for (int i=0; i < xyz.size(); ++i) {
                cout << xyz[i];
                if (i < xyz.size() - 1)
                    cout << ",";
            }
            cout << "] ";
        }
        cout << endl;
#endif
        const auto [width, height] = getDimensions().values();

        // Create a layout consisting completely of walls.
        auto contents = thickmaze::createThickMazeLayout(getDimensions(), CellType::WALL);

        // Find all the rooms as defined by the colouring and the configuration.
        std::vector<std::pair<int, int>> rooms;
        for (auto x = 0; x < width; ++x)
            for (auto y = 0; y < height; ++y)
                if (gridColouring.cellColour(x, y) == configuration.roomColour)
                    rooms.emplace_back(std::make_pair(x, y));

        // Turn walls into offsets, i.e. coordinates in relation to a given coordinate.
        GridColouring::OffsetMap offsetMap = gridColouring.mapWallsToOffsets(configuration);

#ifdef DEBUG
        cout << "OffsetMap:" << endl;
        for (auto om: offsetMap) {
            const auto &[p,q] = om;
            cout << p << ": ";
            for (auto c: q) {
                cout << "(" << c.first << "," << c.second << ") ";
            }
        }
        cout << endl;
        cout.flush();
#endif
        // Maintain a collection of walls to process.
        std::vector<AggregateWall> walls;

        // Pick a cell at random, mark it as floor, and add its walls to the vector.
        auto [sx,sy] = math::RNG::randomElement(rooms);
#ifdef DEBUG
        cout << "Starting at cell " << sx << "," << sy << endl;
#endif

        contents[sx][sy] = CellType::FLOOR;
        auto swalls = adjacentWalls(types::cell(sx, sy), offsetMap);
#ifdef DEBUG
        cout << "\tAdjacent walls:" << endl;
        for (auto swall: swalls) {
            cout << "\t[";
            for (int i=0; i < swall.size(); ++i) {
                cout << "(" << swall[i].first << "," << swall[i].second << ")";
                if (i < swall.size() - 1)
                    cout << ",";
            }
            cout << "]" << endl;
        }
#endif
        for (auto sw: swalls)
            walls.emplace_back(sw);

        while (!walls.empty()) {
#ifdef DEBUG
            cout << "Walls has size: " << walls.size() << endl;
#endif
            // Pick a random wall from the list.
            // We swap it with the end element and remove that for efficiency.
            const auto wallIdx = math::RNG::randomRange(walls.size());
            std::swap(walls[wallIdx], walls.back());
            const auto wall = walls.back();
            walls.pop_back();
#ifdef DEBUG
            cout << "Processing wall: ";
            for (auto xyz: wall) {
                cout << "(" << xyz.first << "," << xyz.second << ") ";
            }
            cout << endl;
#endif
            // Check if one of the rooms adjacent to this wall is uncovered.
            // If so, remove the wall, mark the room as visited by making it a floor, and add its
            // walls to the vector.
            auto arooms = adjacentRooms(wall);
#ifdef DEBUG
            cout << "\tAdjacent rooms: ";
            for (auto curroom: arooms) {
                cout << "(" << curroom.first << "," << curroom.second << ")=" << (contents[curroom.first][curroom.second] == CellType::WALL ? "uncovered" : "covered") << "   ";
            }
            cout << endl;
#endif
            bool uncovered = false;
            for (auto r: arooms) {
                auto [rx, ry] = r;
                if (contents[rx][ry] == CellType::WALL) {
#ifdef DEBUG
                    cout << "\tSetting room " << rx << "," << ry << " to FLOOR" << endl;
#endif
                    contents[rx][ry] = CellType::FLOOR;
                    for (auto rw: wall) {
                        auto [rwx, rwy] = rw;
#ifdef DEBUG
                        cout << "\tSetting wall " << rwx << ","  << rwy << " to FLOOR" << endl;
#endif
                        contents[rwx][rwy] = CellType::FLOOR;
                    }

                    auto rWalls = adjacentWalls(r, offsetMap);
                    for (auto rWall: rWalls) {
#ifdef DEBUG
                        cout << "\tAdding wall: ";
                        for (auto xyz: rWall) {
                            cout << xyz.first << "," << xyz.second << " ";
                        }
                        cout << endl;
#endif
                        walls.emplace_back(rWall);
                    }
                }
            }
        }

        return ThickMaze(width, height, contents);
    }

    boost::optional<GridColouringThickMazeGenerator::AggregateWall>
            GridColouringThickMazeGenerator::offsetToAggregate(const types::Cell &c, const GridColouring::Offsets &offsets) const {
        AggregateWall wall;

        auto [x, y] = c;
        for (auto offset: offsets) {
            auto [dx, dy] = offset;
            int cx = x + dx;
            int cy = y + dy;
            if (!getDimensions().cellInBounds(cx, cy))
                return {};
            wall.emplace_back(std::make_pair(cx, cy));
        }
        return wall;
    }

    types::CellCollection GridColouringThickMazeGenerator::adjacentRooms(const AggregateWall &wall) const noexcept {
        types::CellCollection rooms;
        for (auto w: wall) {
            // Check the colours of all of the cells around w.
            auto nbrs = neighbours(w);
            for (auto n: nbrs) {
                const auto [nx, ny] = n;
                if (gridColouring.cellColour(nx,ny) == configuration.roomColour)
                    rooms.emplace_back(n);
            }
        }

        //assert(rooms.size() > 0 && rooms.size() <= 2);
        return rooms;
    }

    GridColouringThickMazeGenerator::AggregateWallCollection
        GridColouringThickMazeGenerator::adjacentWalls(const types::Cell &c, const GridColouring::OffsetMap &offsetMap) const {
        getDimensions().checkCell(c);

        AggregateWallCollection walls;
        for (const auto &offsetData: offsetMap) {
            const auto &[dir, offsets] = offsetData;
            auto wall = offsetToAggregate(c, offsets);
            if (wall)
                walls.emplace_back(*wall);
        }

        return walls;
    }
}