# CMakeLists.txt
# By Sebastian Raaphorst, 2018.

set(_THICKMAZE_PUBLIC_HEADER_FILES
        CellularAutomatonThickMazeGenerator.h
        GridColouring.h
        StringThickMazeRenderer.h
        ThickMaze.h
        ThickMazeAttributes.h
        ThickMazeGenerator.h
        ThickMazeRenderer.h
        )

set(_THICKMAZE_PRIVATE_HEADER_FILES
        )

set(_THICKMAZE_SOURCE_FILES
        CellularAutomatonThickMazeGenerator.cpp
        GridColouring.cpp
        StringThickMazeRenderer.cpp
        ThickMaze.cpp
        ThickMazeGenerator.cpp
        )

src_dir_filelist("${_THICKMAZE_PUBLIC_HEADER_FILES}" outlst)
set(THICKMAZE_PUBLIC_HEADER_FILES "${outlst}" PARENT_SCOPE)

src_dir_filelist("${_THICKMAZE_PRIVATE_HEADER_FILES}" outlst)
set(THICKMAZE_PRIVATE_HEADER_FILES "${outlst}" PARENT_SCOPE)

src_dir_filelist("${_THICKMAZE_SOURCE_FILES}" outlst)
set(THICKMAZE_SOURCE_FILES "${outlst}" PARENT_SCOPE)
