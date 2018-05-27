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
        AldousBroderMazeGenerator.h
        BFSMazeGenerator.h
        BinaryTreeMazeGenerator.h
        CellularAutomatonThickMazeGenerator.h
        CommonMazeAttributes.h
        DefaultRNG.h
        DFSMazeGenerator.h
        EllerMazeGenerator.h
        Exceptions.h
        GrowingTreeMazeGenerator.h
        Homomorphism.h
        HuntAndKillMazeGenerator.h
        KruskalMazeGenerator.h
        Maze.h
        MazeAttributes.h
        MazeGenerator.h
        MazeRenderer.h
        PrimMazeGenerator.h
        Prim2MazeGenerator.h
        RecursiveDivisionMazeGenerator.h
        RNG.h
        SidewinderMazeGenerator.h
        SpelunkerConfig.h
        StringMazeRenderer.h
        StringThickMazeRenderer.h
        ThickMaze.h
        ThickMazeAttributes.h
        ThickMazeGenerator.h
        ThickMazeRenderer.h
        WilsonMazeGenerator.h
        )

set(SOURCE_FILES
        AldousBroderMazeGenerator.cpp
        BFSMazeGenerator.cpp
        BinaryTreeMazeGenerator.cpp
        CellularAutomatonThickMazeGenerator.cpp
        DefaultRNG.cpp
        DFSMazeGenerator.cpp
        DisjointSetHelper.h
        EllerMazeGenerator.cpp
        GrowingTreeMazeGenerator.cpp
        HuntAndKillMazeGenerator.cpp
        KruskalMazeGenerator.cpp
        Maze.cpp
        MazeGenerator.cpp
        PrimMazeGenerator.cpp
        Prim2MazeGenerator.cpp
        RecursiveDivisionMazeGenerator.cpp
        RNG.cpp
        Show.h
        SidewinderMazeGenerator.cpp
        StringMazeRenderer.cpp
        StringThickMazeRenderer.cpp
        ThickMaze.cpp
        ThickMazeGenerator.cpp
        WilsonMazeGenerator.cpp
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

