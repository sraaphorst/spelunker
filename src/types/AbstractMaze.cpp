/**
 * AbstractMaze.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include "AbstractMaze.h"
#include "CommonMazeAttributes.h"
#include "Dimensions2D.h"
#include "Transformation.h"

namespace spelunker::types {

    AbstractMaze::AbstractMaze(const Dimensions2D &d, const PossibleCell &startPos, const CellCollection &endPos)
            : dimensions{d}, startCell{startPos}, goalCells{endPos} {
        if (d.getWidth() < 1 || d.getHeight() < 1)
            throw types::IllegalDimensions(d.getWidth(), d.getHeight());
        if (startCell)
            checkCell(*startCell);
        for (const auto g: goalCells)
            checkCell(g);
    }

    AbstractMaze::AbstractMaze(const Dimensions2D &d)
            : AbstractMaze{d, {}, CellCollection()} {}

    AbstractMaze::AbstractMaze(const int w, const int h, const PossibleCell &startPos, const CellCollection &endPos)
            : AbstractMaze{Dimensions2D{w, h}, startPos, endPos} {}

    AbstractMaze::AbstractMaze(const int w, const int h)
            : AbstractMaze{Dimensions2D{w, h}, {}, CellCollection()} {}

    AbstractMaze::AbstractMaze::AbstractMaze()
            : dimensions{Dimensions2D{1, 1}}, startCell{}, goalCells{CellCollection{}} {}


    bool AbstractMaze::operator==(const AbstractMaze &other) const noexcept {
        return dimensions == other.dimensions &&
               startCell == other.startCell &&
               goalCells == other.goalCells;
    }


    void AbstractMaze::setStartingCell(const PossibleCell &startPos) noexcept {
        if (startPos)
            checkCell(*startPos);
        startCell = startPos;
    }


    void AbstractMaze::setGoalCells(const CellCollection &cells) noexcept {
        for (const auto c: cells)
            checkCell(c);
        goalCells = cells;
    }


    void AbstractMaze::checkCell(const Cell &c) const {
        dimensions.checkCell(c);
    }


    void AbstractMaze::checkCell(int x, int y) const {
        checkCell(types::cell(x, y));
    }


    bool AbstractMaze::cellInBounds(const Cell &c) const noexcept {
        return dimensions.cellInBounds(c) && numCellWalls(c) < NumWalls;
    }


    bool AbstractMaze::cellInBounds(int x, int y) const noexcept {
        return cellInBounds(cell(x, y));
    }


    const CellCollection AbstractMaze::findDeadEnds() const noexcept {
        CellCollection deadends;
        const auto[width, height] = dimensions.values();
        for (auto y = 0; y < height; ++y) {
            for (auto x = 0; x < width; ++x) {
                const auto c = cell(x, y);
                if (numCellWalls(c) == 3)
                    deadends.emplace_back(c);
            }
        }
        return deadends;
    }


    const CellCollection AbstractMaze::findJunctions() const noexcept {
        CellCollection junctions;
        const auto [width, height] = dimensions.values();
        for (auto y = 0; y < height; ++y) {
            for (auto x = 0; x < width; ++x) {
                const auto c = cell(x, y);
                if (numCellWalls(c) == 0 || numCellWalls(c) == 1)
                    junctions.emplace_back(c);
            }
        }
        return junctions;
    }


    const int AbstractMaze::numCarvedWalls() const noexcept {
        // We do this by iterating over the cells and counting how many walls each cell is missing.
        // Each missing wall falls between two cells, so it will be counted twice. Thus, we must dissolve the
        // total by two.
        int num = 0;
        const auto [width, height] = dimensions.values();
        for (auto y = 0; y < height; ++y)
            for (auto x = 0; x < width; ++x)
                num += (4 - numCellWalls(types::cell(x, y)));
        return num / 2;
    }


    const types::CellSet AbstractMaze::neighbours(const types::CellCollection &cc) const {
        // Add all the neighbours of each cell.
        types::CellSet nbrs;
        for (const auto &c: cc) {
            const auto cnbrs = neighbours(c);
            for (const auto &c2: cnbrs)
                nbrs.insert(c2);
        }

        // Remove the original cells.
        for (const auto &c: cc)
            nbrs.erase(c);

        return nbrs;
    }


    const BFSResults AbstractMaze::performBFSFrom(const types::Cell &start) const {
        checkCell(start);

        // Keep track of the cells to which we're connected.
        CellCollection connectedCells;

        // Keep track of the distances of the cells.
        // start is the only cell at position zero so record it.
        CellDistances distances{1};

        // Prepare the queue for BFS. The cell info comprises a cell and its distance.
        using cellInfo = std::pair<const types::Cell, const int>;
        std::queue<cellInfo> cellQueue{};
        cellQueue.emplace(cellInfo{start, 0});

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
                cellQueue.emplace(cellInfo{n, dist + 1});
        }

        return BFSResults{start, connectedCells, distances};
    }


    const CellCollection AbstractMaze::findInvalidCells() const noexcept {
        CellCollection cc;
        for (auto y = 0; y < getHeight(); ++y)
            for (auto x = 0; x < getWidth(); ++x)
                if (!cellInBounds(x, y))
                    cc.emplace_back(cell(x, y));
        return cc;
    }


    const ConnectedComponents AbstractMaze::findConnectedComponents() const noexcept {
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


    const FurthestCellResults AbstractMaze::findDiameter() const noexcept {
        int longestDistance = 0;
        std::vector<std::pair<Cell, Cell>> winners;
        using cellInfo = std::pair<const Cell, const int>;
        auto ranker = [this](int x, int y) { return y * getWidth() + x; };

        // For speed, use array to keep track of cells visited.
        const auto numCells = getWidth() * getHeight();

        for (auto y = 0; y < getHeight(); ++y) {
            for (auto x = 0; x < getWidth(); ++x) {
                // We can't just clear this: it doesn't work.
                std::vector<bool> visited(numCells, false);

                std::queue<cellInfo> cellQueue;
                types::Cell stc{x,y};
                cellQueue.emplace(cellInfo{stc, 0});

                while (!cellQueue.empty()) {
                    const auto [c, cd] = cellQueue.front();
                    const auto [cx, cy] = c;
                    const auto rk = ranker(cx, cy);
                    cellQueue.pop();
                    if (visited[rk]) continue;

                    visited[rk] = true;
                    if (cd > longestDistance) {
                        longestDistance = cd;
                        winners.clear();
                        winners.emplace_back(std::make_pair(stc, c));
                    } else if (cd == longestDistance) {
                        if (compareCells(stc, c) < 0)
                            winners.emplace_back(std::make_pair(stc, c));
                    }

                    const auto nbrs = neighbours(c);
                    for (const auto &n: nbrs) {
                        cellQueue.emplace(cellInfo{n, cd + 1});
                    }
                }
            }
        }

        return FurthestCellResults{longestDistance, winners};
    }


//    template<typename Archive>
//    void AbstractMaze::serialize(Archive &ar, const unsigned int version) {
//        ar & const_cast<Dimensions2D &>(dimensions);
//        ar & startCell;
//        ar & goalCells;
//    }
}