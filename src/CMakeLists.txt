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
        BinaryTreeMazeGenerator.h
        DefaultRNG.h
        DFSMazeGenerator.h
        EllerMazeGenerator.h
        Exceptions.h
        KruskalMazeGenerator.h
        Maze.h
        MazeAttributes.h
        MazeGenerator.h
        MazeRenderer.h
        PrimMazeGenerator.h
        Prim2MazeGenerator.h
        RNG.h
        StringMazeRenderer.h
        )

set(SOURCE_FILES
        BinaryTreeMazeGenerator.cpp
        DefaultRNG.cpp
        DFSMazeGenerator.cpp
        DisjointSetHelper.h
        EllerMazeGenerator.cpp
        KruskalMazeGenerator.cpp
        Maze.cpp
        MazeGenerator.cpp
        PrimMazeGenerator.cpp
        Prim2MazeGenerator.cpp
        RNG.cpp
        Show.h
        StringMazeRenderer.cpp
        )

add_library(spelunker SHARED
        ${SOURCE_FILES}
        ${PUBLIC_HEADER_FILES}
        )

set(TYPECLASS_HEADER_FILES
        Show.h
        )

install(TARGETS spelunker
        DESTINATION lib
        PUBLIC_HEADER DESTINATION include/spelunker)

target_include_directories(spelunker PUBLIC ${SMAKE_CURRENT_SOURCE_DIR})

