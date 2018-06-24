/**
 * ThickMaze.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <boost/serialization/vector.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <functional>

#include <types/CommonMazeAttributes.h>
#include <types/Dimensions2D.h>
#include <types/Exceptions.h>
#include <types/Symmetry.h>
#include <math/MathUtils.h>
#include <math/RNG.h>

#include "ThickMazeAttributes.h"
#include "ThickMaze.h"

namespace spelunker::thickmaze {
    ThickMaze::ThickMaze(const types::Dimensions2D &d,
                         const types::PossibleCell &start,
                         const types::CellCollection &goals,
                         const CellContents &c)
        : types::AbstractMaze<ThickMaze>{d, start, goals}, contents{c} {

        if (start && cellIs(*start) == CellType::WALL)
            throw types::IllegalSpecialCellPosition{*start, types::SpecialCellType::START};

        for (auto gc: goals)
            if (cellIs(gc) == CellType::WALL)
                throw types::IllegalSpecialCellPosition{gc, types::SpecialCellType::GOAL};
    }

    ThickMaze::ThickMaze(const types::Dimensions2D &d, const thickmaze::CellContents &c)
        : ThickMaze{d, {}, types::CellCollection(), c} {}

    ThickMaze::ThickMaze(const int w, const int h,
                         const types::PossibleCell &start,
                         const types::CellCollection &goals,
                         const CellContents &c)
            : ThickMaze{types::Dimensions2D{w, h}, start, goals, c} {}

    ThickMaze::ThickMaze(const int w, const int h, const CellContents &c)
        : ThickMaze{types::Dimensions2D{w, h}, {}, types::CellCollection{}, c} {}

    bool ThickMaze::operator==(const ThickMaze &other) const noexcept {
        // We can just compare the wall incidence vectors, since == on vectors of the
        // same size compares the contents.
        return types::AbstractMaze<ThickMaze>::operator==(other) &&
               contents == other.contents;
    }

    const CellType ThickMaze::cellIs(int x, int y) const {
        checkCell(x, y);
        return contents[x][y];
    }

    const CellType ThickMaze::cellIs(const spelunker::types::Cell &c) const {
        const auto [x, y] = c;
        return cellIs(x, y);
    }

    int ThickMaze::numCellWalls(const types::Cell &c) const {
        return numCellWallsInContents(c, contents);
    }

    const ThickMaze ThickMaze::applySymmetry(types::Symmetry s) const {
        // Get the symmetry map corresponding to the symmetry.
        std::function<const types::Cell(const types::Cell&)> mp;

        const auto [width, height] = getDimensions().values();

        switch (s) {
            case types::Symmetry::IDENTITY:
                mp = [](const types::Cell &c) {
                    return c;
                };
                break;
            case types::Symmetry::ROTATION_BY_90:
                mp = [this](const types::Cell &c) {
                    const auto[x, y] = c;
                    return types::cell(getHeight() - y - 1, x);
                };
                break;
            case types::Symmetry::ROTATION_BY_180:
                mp = [this](const types::Cell &c) {
                    const auto[x, y] = c;
                    return types::cell(getWidth() - x - 1, getHeight() - y - 1);
                };
                break;
            case types::Symmetry::ROTATION_BY_270:
                mp = [this](const types::Cell &c) {
                    const auto[x, y] = c;
                    return types::cell(y, getWidth() - x - 1);
                };
                break;
            case types::Symmetry::REFLECTION_IN_X:
                mp = [this](const types::Cell &c) {
                    const auto[x, y] = c;
                    return types::cell(x, getHeight() - y - 1);
                };
                break;
            case types::Symmetry::REFLECTION_IN_Y:
                mp = [this](const types::Cell &c) {
                    const auto[x, y] = c;
                    return types::cell(getWidth() - x - 1, y);
                };
                break;
            case types::Symmetry::REFLECTION_IN_NWSE:
                if (!getDimensions().isSquare()) throw types::IllegalGroupOperation(getDimensions(), s);
                mp = [this](const types::Cell &c) {
                    const auto[x, y] = c;
                    return types::cell(y, x);
                };
                break;
            case types::Symmetry::REFLECTION_IN_NESW:
                if (!getDimensions().isSquare()) throw types::IllegalGroupOperation(getDimensions(), s);
                mp = [this](const types::Cell &c) {
                    const auto[x, y] = c;
                    return types::cell(getHeight() - y - 1, getWidth() - x - 1);
                };
        }

        // Determine the new width / height and create the wall incidence.
        const auto nDim = types::applySymmetryToDimensions(s, getDimensions());
        auto ncc = createThickMazeLayout(nDim);

        for (auto y = 0; y < height; ++y) {
            for (auto x = 0; x < width; ++x) {
                const auto [nx, ny] = mp(types::cell(x, y));
                ncc[nx][ny] = cellIs(x, y);
            }
        }

        return ThickMaze{nDim, ncc};
    }

    const ThickMaze ThickMaze::reverse() const noexcept {
        const auto [width, height] = getDimensions().values();

        auto invContents = createThickMazeLayout(width, height);
        for (auto y=0; y < height; ++y)
            for (auto x=0; x < width; ++x)
                invContents[x][y] = contents[x][y] == CellType::FLOOR ?CellType::WALL : CellType::FLOOR;
        return ThickMaze{getDimensions(), invContents};
    }

    const ThickMaze ThickMaze::braid(double probability) const noexcept {
        math::MathUtils::checkProbability(probability);

        // Create a copy of the contents for this maze for the new maze.
        CellContents newContents = contents;

        // Find all the dead ends and store them, shuffle them, and process.
        types::CellCollection deadends = findDeadEnds();
        math::RNG::shuffle(deadends);

        // Lambda function to determine if a cell is a dead end.
        auto isDeadEnd = [&newContents, this](const int x, const int y) {
            if (x < 0 || x >= getWidth() || y < 0 || y >= getHeight() || newContents[x][y] == CellType::WALL)
                return false;
            return numCellWalls(types::cell(x,y)) == 3;
        };

        // Lambda function to determine how many dead ends are around a wall.
        // Returns -1 if the coordinates do not specify a valid wall.
        auto deadEndCounter = [&newContents, isDeadEnd, this](const int x, const int y) {
            if (x < 0 || x >= getWidth() || y < 0 || y >= getHeight() || newContents[x][y] != CellType::WALL)
                return -1;

            int numDeadEnds = 0;
            if (isDeadEnd(x-1, y)) ++numDeadEnds;
            if (isDeadEnd(x+1, y)) ++numDeadEnds;
            if (isDeadEnd(x, y-1)) ++numDeadEnds;
            if (isDeadEnd(x, y+1)) ++numDeadEnds;
            return numDeadEnds;
        };

        for (auto c: deadends) {
            // Check that the probability successds and that thus cell is still a dead end.
            if (math::RNG::randomProbability() > probability || numCellWallsInContents(c, newContents) < 3) {
                continue;
            }
            const auto [x,y] = c;

            // Create a list of the cells around this cell that can be removed and that
            // eliminate the largest number of other dead ends.
            std::vector<types::Cell> candidates;
            int maxDeadEnds = 0;

            auto cellHandler = [&maxDeadEnds, &candidates, deadEndCounter](const int x, const int y) {
                auto dw = deadEndCounter(x, y);
                if (dw < maxDeadEnds)
                    return;

                if (dw > maxDeadEnds) {
                    candidates.clear();
                    maxDeadEnds = dw;
                }
                candidates.emplace_back(types::cell(x,y));
            };

            cellHandler(x-1, y);
            cellHandler(x+1, y);
            cellHandler(x, y-1);
            cellHandler(x, y+1);

            // Now pick a candidate and remove the wall.
            const auto [ex, ey] = math::RNG::randomElement(candidates);
            newContents[ex][ey] = CellType::FLOOR;
        }

        return ThickMaze(getDimensions(), newContents);
    }

    int ThickMaze::numCellWallsInContents(const types::Cell &c, const CellContents &cc) const {
        checkCell(c);
        const auto [x,y] = c;
        if (contents[x][y] == CellType::WALL)
            return 0;

        auto numCellWalls = 0;
        if (x == 0 || cc[x-1][y] == CellType::WALL)             ++numCellWalls;
        if (x == getWidth()-1 || cc[x+1][y] == CellType::WALL)  ++numCellWalls;
        if (y == 0 || cc[x][y-1] == CellType::WALL)             ++numCellWalls;
        if (y == getHeight()-1 || cc[x][y+1] == CellType::WALL) ++numCellWalls;

        return numCellWalls;
    }

    ThickMaze ThickMaze::load(std::istream &s) {
        ThickMaze tm;
        boost::archive::text_iarchive ia{s};
        ia >> tm;
        return tm;
    }

    void ThickMaze::save(std::ostream &s) const {
        boost::archive::text_oarchive oa{s};
        oa << *this;
    }

    template<typename Archive>
    void ThickMaze::serialize(Archive &ar, const unsigned int version) {
        ar & boost::serialization::base_object<types::AbstractMaze<ThickMaze>>(*this);
        ar & const_cast<CellContents&>(contents);
    }

    const types::CellCollection ThickMaze::neighbours(const types::Cell &c) const {
        checkCell(c);
        const auto [x, y] = c;

        types::CellCollection cc;
        for (auto d: types::directions()) {
            const types::Cell nc = types::applyDirectionToCell(c, d);
            const auto [nx, ny] = nc;
            if (cellInBounds(nc) && contents[nx][ny] == CellType::FLOOR)
                cc.emplace_back(nc);
        }

        return cc;
    }
}
