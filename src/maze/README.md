# Maze

In `spelunker`, a `Maze` is a grid of cells with walls being lines separating the cells of the grid. The `maze` library is one of the two representations of such mazes, with the other fully equivalent representation in the [`graphmaze`](../graphmaze/README.md) libbrary.

In the `maze` library, `Maze`s are stored as a collection of possible walls, each with a unique identifier, and then an incidence lookup to determine which walls are and are not present in the `Maze`. Given a `Cell` `(x,y)` and a `Direction` (`NORTH`, `EAST`, `SOUTH`, or `WEST`), the unique wall identifier is calculated for the lookup.

The `maze` library offers a diverse range of algorithms as listed below. Each is described briefly, with an example.

1. [Aldous-Broder Algorithm](#aldous-broder-algorithm)

2. [Random Binary Tree](#random-binary-tree)

3. [Randomized Breadth-First Search](#randomized-breadth-first-search)

4. [Randomized Depth-First Search](#randomized-depth-first-search)

5. [Eller's Algorithm](#ellers-algorithm)

6. [Hunt-and-Kill](#hunt-and-kill)

7. [Randomized Kruskal's Algorithm](#randomized-kruskals-algorithm)

8. [Randomized Prim's Wall Algorithm](#randomized-prims-wall-algorithm)

9. [Randomized Prim's Cell Algorithm](#randomized-prims-cell-algorithm)

10. [Recursive Division](#recursive-division)

11. [Sidewinder Algorithm](#sidewinder-algorithm)

12. [Wilson's Algorithm](#wilsons-algorithm)

## Aldous-Broder Algorithm

## Random Binary Tree

## Randomized Breadth-First Search

## Randomized Depth-First Search

## Eller's Algorithm

## Hunt-and-Kill

## Randomized Kruskal's Algorithm

## Randomized Prim's Wall Algorithm

## Randomized Prim's Cell Algorithm

## Recursive Division

## Sidewinder Algorithm

## Wilson's Algorithm
