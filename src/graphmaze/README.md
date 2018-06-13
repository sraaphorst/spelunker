## GraphMaze

The `GraphMaze` library represents mazes using graphs as implemented in the [Boost.Graph](https://www.boost.org/doc/libs/1_67_0/libs/graph/doc/index.html "Boost.Graph 1.67.0") API.

A maze is represented by an adjacency list, with one vertex per grid cell, beginning in the upper left corner at 0 and increasing along the rows. Edges between vertices represent passages, aka the absence of walls.

A `GraphMaze` is isomorphic to a regular [`Maze`](../maze/README.md), and thus this API has typeclass instances of `Homomorphism` to `Maze` and `Show`, which works via the homomorphism to use `Show<Maze>`.
