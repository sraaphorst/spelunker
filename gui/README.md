# GUI

This is the user interface bundle, designed using Qt 5.9.4, and it is currently a work-in-progress.

It is optional, and does not comprise part of the `spelunker` library.

The goal is to provide a user interface that allows users to:

1. Pick a type of maze (e.g. `Maze` or `ThickMaze`;

2. Pick a `MazeGenerator` and configure the inputs;

3. Generate a maze, which can be serialized or turned into JSON.

4. Tweaked, perhaps by deleting and adding walls.

5. Braided, made unicursal, and made sparse when the options apply.

6. Apply a variety of solving techniques to the maze.

All of this will be graphically demonstrated by a signal / slots mechanism based on Boost.Signals2 and Qt5.

Any further requests / suggestions would be much appreciated. Please contact me at srcoding@gmail.com.
