# CMakeLists.txt
# By Sebastian Raaphorst, 2018.

set(_THICKMAZE_PUBLIC_HEADER_FILES
        CellularAutomatonThickMazeGenerator.h
        GridColouring.h
        GridColouringThickMazeGenerator.h
        StringThickMazeRenderer.h
        ThickMaze.h
        ThickMazeAttributes.h
        ThickMazeGenerator.h
        ThickMazeGeneratorByHomomorphism.h
        ThickMazeRenderer.h
        ThickMazeTypeclasses.h
        PARENT_SCOPE
        )

set(_THICKMAZE_PRIVATE_HEADER_FILES
        PARENT_SCOPE
        )

set(_THICKMAZE_SOURCE_FILES
        CellularAutomatonThickMazeGenerator.cpp
        GridColouring.cpp
        GridColouringThickMazeGenerator.cpp
        StringThickMazeRenderer.cpp
        ThickMaze.cpp
        ThickMazeAttributes.cpp
        ThickMazeGenerator.cpp
        PARENT_SCOPE
        )

