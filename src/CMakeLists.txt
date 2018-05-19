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
        StringMazeRenderer.h
        Exceptions.h
        FullMaze.h
        Maze.h
        MazeAttributes.h
        MazeGenerator.h
        MazeRenderer.h
        RandomizedKruskalMazeGenerator.h)

set(SOURCE_FILES
        StringMazeRenderer.cpp
        Maze.cpp
        DisjointSetHelper.h
        MazeGenerator.cpp
        RandomizedKruskalMazeGenerator.cpp
        Show.h)

add_library(spelunker SHARED
        ${SOURCE_FILES}
        ${PUBLIC_HEADER_FILES})

set(TYPECLASS_HEADER_FILES
        Show.h)

install(TARGETS spelunker
        DESTINATION lib
        PUBLIC_HEADER DESTINATION include/spelunker)

target_include_directories(spelunker PUBLIC ${SMAKE_CURRENT_SOURCE_DIR})

