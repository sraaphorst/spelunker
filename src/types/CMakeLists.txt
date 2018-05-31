# CMakeLists.txt
# By Sebastian Raaphorst, 2018.

configure_file (
        "${CMAKE_CURRENT_SOURCE_DIR}/SpelunkerConfig.h.in"
        "${CMAKE_CURRENT_BINARY_DIR}}/SpelunkerConfig.h"
)
include_directories("${CMAKE_CURRENT_BINARY_DIR}")


set(_TYPES_PUBLIC_HEADER_FILES
        CommonMazeAttributes.h
        Exceptions.h
        SpelunkerConfig.h
        )

set(_TYPES_PRIVATE_HEADER_FILES
        DisjointSetHelper.h
        )

set(_TYPES_SOURCE_FILES
        )

src_dir_filelist("${_TYPES_PUBLIC_HEADER_FILES}" outlst)
set(TYPES_PUBLIC_HEADER_FILES "${outlst}" PARENT_SCOPE)

src_dir_filelist("${_TYPES_PRIVATE_HEADER_FILES}" outlst)
set(TYPES_PRIVATE_HEADER_FILES "${outlst}" PARENT_SCOPE)

src_dir_filelist("${_TYPES_SOURCE_FILES}" outlst)
set(TYPES_SOURCE_FILES "${outlst}" PARENT_SCOPE)
