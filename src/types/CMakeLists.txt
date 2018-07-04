# CMakeLists.txt
# By Sebastian Raaphorst, 2018.

set(_TYPES_PUBLIC_HEADER_FILES
        AbstractMaze.h
        AbstractMazeGenerator.h
        BraidableMaze.h
        CommonMazeAttributes.h
        Dimensions2D.h
        Direction.h
        Exceptions.h
        Observable.h
        Observer.h
        ReversibleMaze.h
        TransformableMaze.h
        Transformation.h
        UnicursalizableMaze.h
        PARENT_SCOPE
        )

set(_TYPES_PRIVATE_HEADER_FILES
        DisjointSetHelper.h
        PARENT_SCOPE
        )

set(_TYPES_SOURCE_FILES
        AbstractMaze.cpp
        CommonMazeAttributes.cpp
        Dimensions2D.cpp
        Direction.cpp
        Transformation.cpp
        PARENT_SCOPE
        )

