# spelunker

A C++17 library for generating / solving mazes. (It could be easily adapted for C++14, but this is a learning exercise for me in C++11, 14, and 17.)

Right now, the library can generate mazes using the following ten algorithms:

1. Aldous-Broder with random walks.

2. Random binary trees (with branching probability).

3. Randomized DFS.

4. Eller's algorithm, allowing probability for horizontal wall creation and density per contiguous block of cells in a row for vertical wall creation.

5. Randomized Kruskal's algorithm.

6. Randomized Prim, iterating over walls.

7. Randomized Prim, iterating over cells.

8. Recursive division, allowing lower bound density and upper bound density for splits, probability to split vertically, and a flag to force alternating vertical / horizontal splits.

9. Sidewinder algorithm.

10. Wilson's algorithm.

It also defines a type class, `Show`, that can be used to easily create string representations of mazes, coordinates, positions, etc. for text output.

# Requirements

A C++17 capable compiler (unless you want to go through and nest the namespaces), Boost 1.31 or higher (for `disjoint_sets.hpp`), and cmake 3.10 or higher.

# Further work

1. Include more maze generation algorithms.

2. Add the ability for listeners to subcribe to `MazeGenerator` instances to receive events when a maze is extended. This will allow, say, drawing of a maze as it is being generated to show how the algorithms work.

3. Add testing with: https://github.com/catchorg/Catch2

4. CI: configure Travis.

5. Add various maze solvers, and also make them subscribable so that it is possible to draw, step-by-step, the path taken through the maze.

5. Write a Qt UI (as a binary independent of the library) to display all these features (maze step-by-step generation, maze step-by-step solving, etc) in a visually pleasant way.

6. Be able to somehow serialize mazes, preferably in JSON?

# Example

Here is an example of a width 50, height 40 maze generated by the randomized DFS algorithm:

```
┌───────┬───────────┬───────────┬───────────┬───┬───────────┬─────┬─────────┬─────────┬───────┬─────┐ 
│ ╶─┐ ╷ ╵ ┌───┬─╴ ┌─┘ ╷ ╶─┬─────┘ ┌───┐ ┌─┐ ╵ ╷ │ ╶─┐ ┌───┐ ╵ ┌───┘ ╷ ┌───╴ │ ╷ ╶─┬─╴ │ ┌─┬─╴ │ ┌─┐ │ 
├─┐ │ └─┬─┘ ┌─┘ ┌─┘ ┌─┼─╴ │ ╶───┬─┘ ╷ │ │ └───┘ └───┘ │ ╷ ├───┤ ╶───┤ └─────┘ ├─╴ │ ╶─┘ │ │ ╶─┤ │ ╵ │ 
│ ╵ └─┐ │ ╷ │ ╶─┤ ┌─┘ │ ╶─┴───┐ ╵ ┌─┤ │ └───┐ ┌─────┐ │ │ ╵ ╷ └─╴ ┌─┴─────┐ ╶─┤ ┌─┴───┬─┘ └─┐ │ ├───┤ 
│ ┌─┬─┘ │ ├─┴─┐ │ ╵ ╷ └─┬─┐ ╶─┴───┘ │ ├───┐ └─┤ ┌─┐ ├─┘ └─┬─┴─────┘ ┌───┐ │ ┌─┘ │ ╶─┐ │ ┌─╴ │ │ ╵ ╷ │ 
│ │ │ ┌─┤ ╵ ╷ ╵ ├─┬─┴─┐ │ └───┬─╴ ┌─┘ ├─╴ └─┐ ╵ ╵ │ ╵ ┌───┘ ┌───────┘ ╷ ╵ └─┘ ┌─┴─╴ │ │ ├───┤ └─┐ │ │ 
│ │ │ │ │ ╶─┴───┘ │ ╷ │ └─╴ ┌─┘ ┌─┘ ┌─┘ ┌─┐ └───┬─┴─┬─┘ ╷ ┌─┴─┐ ┌───┬─┴─┬─╴ ┌─┘ ┌───┤ │ ╵ ╷ └─╴ │ │ │ 
│ ╵ │ │ ╵ ┌───────┘ │ └─┬─╴ │ ╶─┘ ┌─┘ ┌─┘ └───┐ │ ╷ ╵ ╷ ├─┘ ╷ ├─┘ ╷ ╵ ╷ └─┐ │ ┌─┘ ╷ │ ├───┴───┐ │ │ │ 
│ ┌─┘ │ ╶─┤ ┌───────┤ ╷ │ ╶─┼─────┘ ┌─┘ ╷ ┌─╴ │ │ └─┬─┘ │ ┌─┤ ╵ ╶─┼───┤ ╷ │ │ └───┤ │ │ ┌───╴ │ └─┘ │ 
│ │ ┌─┴───┤ └─┐ ╷ ┌─┘ ├─┴─╴ │ ╷ ┌───┴─┐ │ └─┐ │ └─┐ └─┐ │ │ └─┬───┘ ╷ └─┘ ├─┴───╴ ╵ │ ╵ │ ╶───┼───╴ │ 
│ │ ╵ ┌─╴ ├─┐ └─┤ │ ╶─┤ ╶───┴─┤ ╵ ┌─╴ ├─┴─┐ └─┴─╴ ├─╴ │ │ │ ╶─┤ ╶─┬─┴─┬───┘ ╶─┬─────┴─┐ ├───┐ │ ╶───┤ 
│ ├───┘ ╷ ╵ ├─┐ ╵ ├─╴ ├─────┐ │ ╶─┤ ╷ │ ╷ │ ╶─────┤ ┌─┘ │ └─╴ └─┐ ╵ ╷ ╵ ╶─┬───┴─────╴ │ └─┐ │ └─┬─╴ │ 
│ │ ┌───┴─┐ │ └─┐ │ ╶─┤ ┌─┐ │ ├─╴ │ └─┘ │ └─┬───┐ │ └─┐ └───┐ ┌─┴───┤ ┌─┬─┘ ┌───────┬─┴─┐ ╵ ├─╴ ├───┤ 
├─┘ │ ╶───┘ ╵ ╷ │ └─┐ │ │ │ │ └───┴─┐ ╶─┴─┐ │ ╶─┘ └─┐ │ ┌───┘ │ ┌─┐ │ ╵ │ ┌─┘ ┌─┐ ┌─┘ ╷ └─┐ │ ╶─┘ ╷ │ 
│ ┌─┘ ┌───────┴─┴─┐ │ ╵ │ │ └───┬─╴ ├───┐ │ └───────┤ └─┘ ┌───┘ │ │ └───┤ │ ╶─┘ │ ╵ ┌─┤ ╶─┘ └─┬───┤ │ 
│ │ ╶─┤ ╶───┬─┬─╴ │ └─┬─┘ │ ╶─┐ │ ╶─┘ ╷ │ ├─────┬─┐ └─┬───┘ ┌───┤ └───┐ ╵ ├───┐ ├───┘ └─────┬─┘ ╷ ╵ │ 
│ └───┴───┐ │ │ ╷ ├─╴ ╵ ╷ ├─╴ │ └─┬───┤ │ ╵ ┌─╴ ╵ └─┐ │ ╶─┬─┘ ╷ └───╴ └─┬─┤ ╷ │ ├─────╴ ╷ ┌─┘ ┌─┼─╴ │ 
│ ┌─────┐ ╵ │ │ └─┤ ╶─┬─┴─┘ ┌─┴─╴ └─╴ │ └─┬─┘ ┌─────┘ ├─╴ │ ┌─┼───────┐ │ ╵ │ ╵ │ ┌─────┤ │ ┌─┘ │ ╶─┤ 
│ └─╴ ╷ └───┤ ├─╴ ├─╴ │ ╶───┴───┬─┬─╴ ├─╴ ├───┘ ┌───┐ │ ╶─┘ │ ╵ ╶─┐ ╶─┤ └─┐ └───┘ │ ┌─┐ ╵ │ │ ╶─┴─╴ │ 
├───┬─┴───┐ ╵ │ ╶─┤ ┌─┴───┬─┬─╴ │ ╵ ╶─┤ ┌─┘ ╶───┤ ╶─┤ ├───┬─┴───┐ └─┐ └─┐ └─┬─────┤ │ └───┤ └─┬─────┤ 
│ ╷ │ ╶─┐ └─╴ ├─┐ ╵ │ ╶─┐ ╵ │ ╶─┴─┬───┤ └───┬─╴ │ ╷ ╵ │ ╶─┘ ╷ ╷ └─┬─┘ ╷ └─┐ └─┐ ╷ │ └─┐ ╷ ├─╴ │ ╶─┐ │ 
│ │ └─╴ ├─────┘ └───┴─┐ └─┐ ├───┐ ╵ ╷ ├───┐ ╵ ┌─┤ └───┴─────┘ ├─┐ │ ┌─┴───┴─┐ │ └─┴─╴ ╵ │ │ ╷ ├─╴ │ │ 
│ ├─────┴───╴ ┌─────┐ │ ╷ └─┤ ╷ └───┤ ╵ ╷ └───┘ ├─────────────┤ ╵ │ ╵ ┌─╴ ╷ │ └───────┐ ├─┘ ├─┘ ┌─┘ │ 
│ └─┐ ┌───────┤ ╶─┐ ╵ │ └─┐ ╵ ├───┐ ├───┴───┐ ╶─┘ ╷ ┌───┬───╴ │ ┌─┴───┤ ┌─┘ └───────┐ ├─┘ ╷ │ ┌─┤ ╶─┤ 
├─┐ ╵ │ ┌───┐ └─┐ └─┬─┘ ┌─┴─┐ ╵ ╷ │ ╵ ╷ ╶─┐ ├─────┤ │ ╶─┤ ╶───┤ ╵ ┌─┐ ╵ ├───────┬───┘ │ ╶─┴─┘ ╵ ├─╴ │ 
│ └───┘ │ ╷ └─╴ ├─╴ │ ╶─┤ ╷ ├───┤ └───┴─┐ │ ├─╴ ╷ │ │ ╷ └─╴ ┌─┴───┘ ╵ ┌─┘ ┌───┐ ╵ ┌───┴─────────┘ ┌─┤ 
│ ┌─┐ ╶─┤ └─┬───┘ ┌─┘ ┌─┘ │ │ ╶─┴─╴ ┌───┘ │ │ ╶─┴─┤ │ └─┬───┘ ┌───────┘ ┌─┘ ╷ └───┘ ┌─────────┬───┘ │ 
│ │ ├─╴ │ ┌─┘ ┌───┴───┘ ┌─┤ └───┬───┤ ┌───┤ └───╴ │ ├─╴ │ ┌───┤ ┌───────┴─┐ └───┐ ╶─┤ ╷ ┌───┐ │ ╶─┐ │ 
│ │ │ ╶─┤ │ ╶─┴───╴ ┌───┘ └───┐ ╵ ╷ ╵ │ ╷ └─────┐ │ │ ┌─┘ ├─╴ │ ├─────╴ ╷ ├───╴ ├─╴ │ │ │ ╷ │ ├─╴ │ │ 
│ │ └─┐ └─┴─────┬───┘ ╶───┬─┐ ├───┴───┴─┘ ╷ ┌───┤ │ │ │ ┌─┘ ╷ │ ╵ ┌─┬───┤ └─┬───┤ ┌─┘ ├─┘ │ └─┤ ┌─┘ │ 
│ ╵ ╷ ├───┐ ┌─╴ │ ╶─┬───┐ │ ╵ └─╴ ┌───────┴─┘ ╷ ╵ │ └─┘ │ ╷ │ └─┬─┘ ╵ ╷ └─┐ │ ╷ │ └─┐ ╵ ┌─┴─┐ ╵ │ ╶─┤ 
├───┤ └─╴ │ └─┐ ├─╴ ├─╴ │ └───────┘ ┌─────────┼───┴─────┴─┘ ├─┐ │ ╶───┼─╴ │ ╵ │ └─┐ │ ╶─┴─╴ └─┬─┴─┐ │ 
├─╴ ├───┐ └─┐ │ │ ┌─┘ ╷ └───┬─────┬─┴─────┐ ╷ └───┐ ┌───┬───┘ │ └───┐ │ ╷ └───┼─╴ │ └─┬─┬───╴ └─╴ │ │ 
│ ┌─┘ ╷ ╵ ┌─┘ │ │ ╵ ┌─┴───┐ │ ╶───┘ ╷ ┌─┐ ╵ ├───┐ ╵ │ ┌─┘ ╷ ╶─┴───┐ │ │ │ ┌───┘ ┌─┴─╴ │ ╵ ┌───────┘ │ 
│ ╵ ┌─┴───┘ ┌─┴─┴───┘ ┌───┤ │ ┌─────┘ │ └───┘ ┌─┴───┤ ╵ ┌─┴───┐ ┌─┘ │ │ └─┤ ┌───┤ ╶───┴───┤ ┌─────┐ │ 
│ ╶─┤ ╷ ┌───┘ ┌───┐ ╶─┤ ╷ │ └─┤ ┌─────┘ ╶─────┤ ╷ ╶─┘ ┌─┘ ┌─╴ │ │ ╶─┴─┤ ╷ │ │ ╶─┘ ┌─┬───╴ │ │ ┌─┐ ╵ │ 
├───┘ │ │ ╶───┘ ╷ ├─┐ ╵ │ └─╴ │ └───┐ ╶───┬─┐ │ ├─────┴─╴ │ ╶─┤ └─┬─┐ └─┤ │ │ ╶───┘ │ ┌───┘ │ │ └───┤ 
│ ╶───┴─┴───┬───┤ ╵ └───┴─┬───┘ ┌─╴ ├───┐ │ │ ╵ │ ╶─────┬─┴─┐ │ ╷ ╵ └─┐ ╵ │ └─┬───┬─┘ │ ╶─┐ │ ├───╴ │ 
│ ┌───┐ ┌─╴ │ ╷ ├───────┐ └─────┤ ┌─┘ ╷ │ ╵ ├───┴───┬─╴ │ ╶─┘ │ │ ┌───┴─┐ └─┐ ╵ ╷ ╵ ┌─┴─╴ │ │ ╵ ┌─┐ │ 
│ ╵ ╷ │ └───┘ │ ╵ ╶─┐ ╶─┴─────╴ ╵ │ ╷ └─┴─╴ └─╴ ┌─╴ ╵ ┌─┘ ╶───┤ └─┘ ╷ ╶─┴─╴ └───┴───┴─────┘ ├───┘ ╵ │ 
└───┴─┴───────┴─────┴─────────────┴─┴───────────┴─────┴───────┴─────┴───────────────────────┴───────┘ 
```

# References

1. http://www.astrolog.org/labyrnth/algrithm.htm

2. https://en.wikipedia.org/wiki/Maze_generation_algorithm

3. https://en.wikipedia.org/wiki/Maze_solving_algorithm

4. http://weblog.jamisbuck.org/2010/12/29/maze-generation-eller-s-algorithm

5. Jamis Buck. Mazes for Programmers. The Pragmatic Programmers, July 2015. http://mazesforprogrammers.com
