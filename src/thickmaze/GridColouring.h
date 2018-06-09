/**
 * GridColouring.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * Represents a periodic colouring of a grid, which we can use to make mazes.
 */

#pragma once

#include <functional>
#include <tuple>
#include <vector>

#include "math/Partition.h"

class GridColouring final {
public:
    using Colour = int;
    using ColourCollection = std::vector<int>;
    using Neighbourhood = ColourCollection;
    using Partition = ColourCollection;
    using Partitions = std::vector<Partition>;
    using ColourLookupRow = std::vector<Colour>;
    using ColourLookup = std::vector<ColourLookupRow>;
    using ReverseLookup = std::vector<std::pair<int, int>>;

    /**
     * This is a candidate that satisfies the three requirements for a maze algorithm:
     * 1. Every room (represented by roomColour
     */
    struct CandidateConfiguration {
        Colour roomColour;
        std::vector<ColourCollection> walls;
    };

    /**
     * When generating CandidateConfigurations, if the supplied function returns true
     * for the currently generated CandidateConfiguration, the algorithm stops and returns only the
     * configurations generated so far.
     */
    using AbortPrematurely = std::function<bool(const CandidateConfiguration&)>;

    /**
     * The parameters to specify a ux * vy colouring K of a grid.
     * They represent two vectors, namely (ux, 0) and (vx, vy) so that colours repeat over these vectors, i.e.:
     * Given any cell c = (x,y), for all integers m and n:
     *    K(x,y) = K(x + m * ux + n * vx, y + n * vy).
     * @param p_ux the x-component of the vector u = (ux, 0)
     * @param p_vx the x-component of the vector v = (vx, vy)
     * @param p_vy the y-component of the vector v = (vx, vy)
     */
    GridColouring(int p_ux, int p_vx, int p_vy);

    ~GridColouring() = default;

    static inline AbortPrematurely maxWeight(int maxWeight) {
        return [maxWeight](const CandidateConfiguration &c) {
            auto weight = 0;
            for (const auto wall: c.walls)
                weight += wall.size();
            return weight <= maxWeight;
        };
    }

    static AbortPrematurely stopAfterRoom0;
    static AbortPrematurely neverStop;

    /**
     * Determine the candidate configurations for maze construction.
     * This comprises colour representing rooms, and a collection of lists of colours comprising walls.
     * (Each "wall" may consist of more than one cell.)
     * Any colours not appearing in the room colours or the wall colours are considered pillars.
     *
     * The candidates have the following properties:
     * 1. The cells comprising a wall are contiguous.
     * 2. The room colour is adjacent to exactly four walls, i.e. its neighbour colours are each in a distinct wall.
     * 3. Every wall is adjacent to two rooms.
     * 4. A wall is adjacent to a room iff that room is adjacent to the wall.
     * @param maxSize the maximum number of candidates to generate, or -1 to continue until all have been generated
     * @param aborter a function that allows premature abortion of the process based on the current candidate
     * @return a list of viable CandidateConfigurations
     */
    std::vector<CandidateConfiguration> wallCandidates(int maxSize = -1,
                                                       AbortPrematurely &aborter = neverStop) const;


    /**
     * Return the colour of the cell (x,y).
     * @param x the x coordinate of the cell
     * @param y the y coordinate of the cell
     * @return the colour of (x,y)
     */
    inline Colour cellColour(int x, int y) const {
        return cellColours[mod(x, ux)][mod(y, rows)];
    }

    /**
     * Given a collection of cell colours, determine the cell colours in their neighbourhood.
     * We represent this as a vector instead of a set because the number of times a colour
     * appears in the neighbourhood is important.
     *
     * Note that if colours has size > 1, the cells will report each other as neighbours:
     * this should not be a problem for our purposes.
     *
     * @param colours the colours to look up
     * @return a list of all colours adjacent to a colour in the list, with repetitions allowed
     */
    Neighbourhood neighbourhood(const ColourCollection &colours) const;

private:
    /**
     * Return the positive value of x mod y, i.e. a value in [0,y).
     * @return the positive value of x mod y
     */
    static inline int mod(int x, int y) {
        return (((x % y) + y) % y);
    }

    const int ux, vx, vy;
    const int numColours;
    const int rows;

    /**
     * A ux x (ux * vy / gcd(ux, vy)) lookup vector of colours of cells, which determines via mod the
     * colours of all cells.
     */
    ColourLookup cellColours;

    /**
     * A reverse lookup: given a colour, return a representative cell containing that colour.
     */
    ReverseLookup reverseLookup;


    /**
     * Determine if a collection of cell representatives are contiguous.
     * This allows us to reject wall candidates that are not contiguous and should not be considered wall segments,
     * such as two cells touching diagonally.
     *
     * The algorithm works as follows:
     * 1. Begin with all cells marked as unvisited.
     * 2. Add the first cell to a queue.
     * 3. While there are cells in the queue, take the top cell, mark it as visited, and add its neighbours in the
     *     collection to the queue.
     * 4. Check if any cells are unmarked: if so, the collection is not contiguous.
     *
     * @param colours the colour representatives to check
     * @return true if the cells are contiguous, and false otherwise.
     */
    bool isContiguous(const ColourCollection &colours) const;
};