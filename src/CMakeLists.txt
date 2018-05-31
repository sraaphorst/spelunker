# CMakeLists.txt
# By Sebastian Raaphorst, 2018.

configure_file (
    "types/SpelunkerConfig.h.in"
    "types/SpelinkerConfig.h"
    )

set(PUBLIC_HEADER_FILES
        maze/AldousBroderMazeGenerator.h
        maze/BFSMazeGenerator.h
        maze/BinaryTreeMazeGenerator.h
        thickmaze/CellularAutomatonThickMazeGenerator.h
        types/CommonMazeAttributes.h
        math/DefaultRNG.h
        maze/DFSMazeGenerator.h
        maze/EllerMazeGenerator.h
        types/Exceptions.h
        maze/GrowingTreeMazeGenerator.h
        typeclasses/Homomorphism.h
        maze/HuntAndKillMazeGenerator.h
        maze/KruskalMazeGenerator.h
        maze/Maze.h
        maze/MazeAttributes.h
        maze/MazeGenerator.h
        maze/MazeRenderer.h
        maze/PrimMazeGenerator.h
        maze/Prim2MazeGenerator.h
        maze/RecursiveDivisionMazeGenerator.h
        math/RNG.h
        maze/SidewinderMazeGenerator.h
        types/SpelunkerConfig.h
        maze/StringMazeRenderer.h
        thickmaze/StringThickMazeRenderer.h
        thickmaze/ThickMaze.h
        thickmaze/ThickMazeAttributes.h
        thickmaze/ThickMazeGenerator.h
        thickmaze/ThickMazeRenderer.h
        maze/WilsonMazeGenerator.h
        )

set(SOURCE_FILES
        maze/AldousBroderMazeGenerator.cpp
        maze/BFSMazeGenerator.cpp
        maze/BinaryTreeMazeGenerator.cpp
        thickmaze/CellularAutomatonThickMazeGenerator.cpp
        math/DefaultRNG.cpp
        maze/DFSMazeGenerator.cpp
        types/DisjointSetHelper.h
        maze/EllerMazeGenerator.cpp
        maze/GrowingTreeMazeGenerator.cpp
        maze/HuntAndKillMazeGenerator.cpp
        maze/KruskalMazeGenerator.cpp
        maze/Maze.cpp
        maze/MazeGenerator.cpp
        maze/PrimMazeGenerator.cpp
        maze/Prim2MazeGenerator.cpp
        maze/RecursiveDivisionMazeGenerator.cpp
        math/RNG.cpp
        typeclasses/Show.h
        maze/SidewinderMazeGenerator.cpp
        maze/StringMazeRenderer.cpp
        thickmaze/StringThickMazeRenderer.cpp
        thickmaze/ThickMaze.cpp
        thickmaze/ThickMazeGenerator.cpp
        maze/WilsonMazeGenerator.cpp
        )

add_library(spelunker SHARED
        ${SOURCE_FILES}
        ${PUBLIC_HEADER_FILES}
        )

set(TYPECLASS_HEADER_FILES
        typeclasses/Show.h
        )

install(TARGETS spelunker
        DESTINATION lib
        PUBLIC_HEADER DESTINATION include/spelunker)

target_include_directories(spelunker PUBLIC ${SMAKE_CURRENT_SOURCE_DIR})

