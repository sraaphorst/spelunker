# CMakeLists
# By Sebastian Raaphorst, 2018.

set(_GRAPHMAZE_PUBLIC_HEADER_FILES
        GraphMaze.h
        GraphMazeAttributes.h
        GraphMazeTypeclasses.h
        )

set(_GRAPHMAZE_PRIVATE_HEADER_FILES
        )

set(_GRAPHMAZE_SOURCE_FILES
        GraphMaze.cpp
        )

src_dir_filelist("${_GRAPHMAZE_PUBLIC_HEADER_FILES}" outlst)
set(GRAPHMAZE_PUBLIC_HEADER_FILES "${outlst}" PARENT_SCOPE)

src_dir_filelist("${_GRAPHMAZE_PRIVATE_HEADER_FILES}" outlst)
set(GRAPHMAZE_PRIVATE_HEADER_FILES "${outlst}" PARENT_SCOPE)

src_dir_filelist("${_GRIDMAZE_SOURCE_FILES}" outlst)
set(GRAPHMAZE_SOURCE_FILES "${outlst}" PARENT_SCOPE)
