# CMakeLists.txt
# By Sebastian Raaphorst, 2018.

set(SUBDIRS
        math
        maze
        thickmaze
        typeclasses
        types)

foreach(d ${SUBDIRS})
    add_subdirectory(${d})
endforeach()

set(PUBLIC_HEADER_FILES
        ${MATH_PUBLIC_HEADER_FILES}
        ${MAZE_PUBLIC_HEADER_FILES}
        ${THICKMAZE_PUBLIC_HEADER_FILES}
        ${TYPECLASSES_PUBLIC_HEADER_FILES}
        ${TYPES_PUBLIC_HEADER_FILES}
        )

set(PRIVATE_HEADER_FILES
        ${MATH_PRIVATE_HEADER_FILES}
        ${MAZE_PRIVATE_HEADER_FILES}
        ${THICKMAZE_PRIVATE_HEADER_FILES}
        ${TYPECLASSES_PRIVATE_HEADER_FILES}
        ${TYPES_PRIVATE_HEADER_FILES}
        )

set(SOURCE_FILES
        ${MATH_SOURCE_FILES}
        ${MAZE_SOURCE_FILES}
        ${TYPES_SOURCE_FILES}
        ${THICKMAZE_SOURCE_FILES}
        ${TYPECLASSES_SOURCE_FILES}
        ${TYPES_SOURCE_FILES}
        )

message("PUBLIC_HEADER_FILES: ${PUBLIC_HEADER_FILES}\n")
message("PRIVATE_HEADER_FILES: ${PRIVATE_HEADER_FILES}\n")
message("SOURCE_FILES: ${SOURCE_FILES}\n")

add_library(spelunker SHARED
        ${PUBLIC_HEADER_FILES}
        ${PRIVATE_HEADER_FILES}
        ${SOURCE_FILES}
        )

install(TARGETS spelunker
        DESTINATION lib
        PUBLIC_HEADER DESTINATION include/spelunker)

target_include_directories(spelunker PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

