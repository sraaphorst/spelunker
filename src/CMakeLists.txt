# CMakeLists.txt
# By Sebastian Raaphorst, 2018.

set (SPELUNKER_VERSION_MAJOR 0)
set (SPELUNKER_VERSION_MINOR 1)
set (SPELUNKER_VERSION_BUILD 0)

configure_file (
    "SpelunkerConfig.h.in"
    "SpelinkerConfig.h"
    )

set(PUBLIC_HEADER_FILES
        Exceptions.h
        Maze.h
        MazeAttributes.h
        RandomizedKruskalMazeGenerator.h)

set(SOURCE_FILES
        Maze.cpp RandomizedKruskalMazeGenerator.cpp DisjointSetHelper.h MazeGenerator.cpp MazeGenerator.h)

add_library(spelunker SHARED
        ${SOURCE_FILES}
        ${PUBLIC_HEADER_FILES})

set(TYPECLASS_HEADER_FILES
        Show.h)

install(TARGETS spelunker
        DESTINATION lib
        PUBLIC_HEADER DESTINATION include/spelunker)

