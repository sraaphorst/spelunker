# CMakeLists.txt
# By Sebastian Raaphorst, 2018.

set(_TYPES_PUBLIC_HEADER_FILES
        AbstractMaze.h
        AbstractMazeGenerator.h
        CommonMazeAttributes.h
        Dimensions2D.h
        Direction.h
        Exceptions.h
        ReversibleMaze.h
        Symmetry.h
        UnicursalizableMaze.h
        PARENT_SCOPE
        )

set(_TYPES_PRIVATE_HEADER_FILES
        DisjointSetHelper.h
        PARENT_SCOPE
        )

set(_TYPES_SOURCE_FILES
        CommonMazeAttributes.cpp
        Dimensions2D.cpp
        Direction.cpp
        Symmetry.cpp
        PARENT_SCOPE
        )

