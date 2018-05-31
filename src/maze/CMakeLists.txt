# CMakeLists.txt
# By Sebastian Raaphorst, 2018.

set(_MAZE_PUBLIC_HEADER_FILES
        AldousBroderMazeGenerator.h
        BFSMazeGenerator.h
        BinaryTreeMazeGenerator.h
        DFSMazeGenerator.h
        EllerMazeGenerator.h
        GrowingTreeMazeGenerator.h
        HuntAndKillMazeGenerator.h
        KruskalMazeGenerator.h
        Maze.h
        MazeAttributes.h
        MazeGenerator.h
        MazeRenderer.h
        PrimMazeGenerator.h
        Prim2MazeGenerator.h
        RecursiveDivisionMazeGenerator.h
        SidewinderMazeGenerator.h
        StringMazeRenderer.h
        WilsonMazeGenerator.h
        )

set(_MAZE_PRIVATE_HEADER_FILES
        )

set(_MAZE_SOURCE_FILES
        AldousBroderMazeGenerator.cpp
        BFSMazeGenerator.cpp
        BinaryTreeMazeGenerator.cpp
        DFSMazeGenerator.cpp
        EllerMazeGenerator.cpp
        GrowingTreeMazeGenerator.cpp
        HuntAndKillMazeGenerator.cpp
        KruskalMazeGenerator.cpp
        Maze.cpp
        MazeGenerator.cpp
        PrimMazeGenerator.cpp
        Prim2MazeGenerator.cpp
        RecursiveDivisionMazeGenerator.cpp
        SidewinderMazeGenerator.cpp
        StringMazeRenderer.cpp
        WilsonMazeGenerator.cpp
        )

src_dir_filelist("${_MAZE_PUBLIC_HEADER_FILES}" outlst)
set(MAZE_PUBLIC_HEADER_FILES "${outlst}" PARENT_SCOPE)

src_dir_filelist("${_MAZE_PRIVATE_HEADER_FILES}" outlst)
set(MAZE_PRIVATE_HEADER_FILES "${outlst}" PARENT_SCOPE)

src_dir_filelist("${_MAZE_SOURCE_FILES}" outlst)
set(MAZE_SOURCE_FILES "${outlst}" PARENT_SCOPE)