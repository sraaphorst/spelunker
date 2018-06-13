# CMakeLists
# By Sebastian Raaphorst, 2018.

set(_GRIDMAZE_PUBLIC_HEADER_FILES
        GraphMaze.h
        GraphMazeAttributes.h
        GraphMazeTypeclasses.h
        )

set(_GRIDMAZE_PRIVATE_HEADER_FILES
        )

set(_GRIDMAZE_SOURCE_FILES
        GraphMaze.cpp
        )

src_dir_filelist("${_GRIDMAZE_PUBLIC_HEADER_FILES}" outlst)
set(GRIDMAZE_PUBLIC_HEADER_FILES "${outlst}" PARENT_SCOPE)

src_dir_filelist("${_GRIDMAZE_PRIVATE_HEADER_FILES}" outlst)
set(GRIDMAZE_PRIVATE_HEADER_FILES "${outlst}" PARENT_SCOPE)

src_dir_filelist("${_GRIDMAZE_SOURCE_FILES}" outlst)
set(GRIDMAZE_SOURCE_FILES "${outlst}" PARENT_SCOPE)
