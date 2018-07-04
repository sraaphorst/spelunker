/**
 * SquashedMaze.h
 *
 * By Sebastian Raaphorst, 2018.
 *
 * A squashed maze is a representation of a maze as a graph that maintains many of its fundamental characteristics.
 * The goal is to create a basic graph that preserves distance between the "important" vertices, i.e. the only
 * vertices where distance could be maximized or involved in the choice of path.
 */

#pragma once

#include <maze/Maze.h>
#include <types/CommonMazeAttributes.h>
#include <types/AbstractMaze.h>
#include "SquashedMazeAttributes.h"


namespace spelunker::squashedmaze {
    /**
     * A SquashedMaze is a minimal representation of a maze as a graph for statistical and algorithmic purposes.
     *
     * Given an AbstractMaze, the squashed maze has one vertex for:
     * 1. Each dead end in the original maze;
     * 2. Each junction (or decision point) in the original maze;
     * 3. Each "entrance cell" inside a room found by the RoomFinder; and
     * 4. Each isolated loop / isolated vertex in the maze.
     *
     * Each edge covers a number of cells in the initial graph: we maintain this information as well. Some cells
     * appear in multiple edges, i.e. the cells at junctures appear in the 3-4 edges they represent.
     *
     * Edges are weighted by the number of moves they take to get from the first cell to the terminal cell.
     * If more than one path exists between two cells, the one with the lower weight is represented.
     *
     * Note that a SquashedMaze does not uniquely represent a maze, i.e. multiple mazes may generate the same
     * SquashedMaze. Like a hashcode, however, if two mazes generate different SquashedMazes, they are not equal.
     *
     * This class also provides us with a number of statistics:
     * 1. Number of passages, as well as their length, with the exception that cells with two paths between them
     *    are shrunk down to one path.
     * 2. Number of T and + junctions.
     * 3. Number of dead ends.
     * 3. Number of rooms and their size.
     */
    class SquashedMaze {
    public:
        /**
         * Given an AbstractMaze, use it to create the representative SquashedMaze, as detailed above.
         * @param m the original maze
         */
        explicit SquashedMaze(const types::AbstractMaze &m);

        ~SquashedMaze() = default;

        /// Return the mapping from graph edge to the cells in the original maze.
        inline const EdgeCellMap &getEdgeMap() const noexcept {
            return edges;
        }

        /// Return the mapping from graph vertex to the corresponding cell in the original maze.
        inline const CellVertexMap &getVertexMap() const noexcept {
            return vertexCell;
        }

        /// Get the weighted graph representing the squashing of the maze.
        inline const WeightedGraph &getGraph() const noexcept {
            return graph;
        }

    private:
        /**
         * Process a room as found by @see[RoomFinder].
         * For each room, we:
         * 1. Find the entrances, and add a vertex for each;
         * 2. Find the shortest path between each of the entrances and add an edge for each;
         * @tparam T the maze type
         * @param m the maze
         * @param cc the collection of cells comprising the room
         * @return the list of entrance cells for the room
         */
        types::CellCollection processRoom(const types::AbstractMaze &m, const types::CellCollection &cc);

        /// Each edge covers multiple cells. We map between cells and edges.
        EdgeCellMap edges;

        /// Each vertex of the squashed maze is associated with a cell in the original maze.
        CellVertexMap vertexCell;

        /// The graph that represents the squashed maze.
        WeightedGraph graph;
    };
}

