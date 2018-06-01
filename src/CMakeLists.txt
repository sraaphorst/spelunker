# CMakeLists.txt
# By Sebastian Raaphorst, 2018.

project(spelunker VERSION 0.1.0 LANGUAGES CXX)

configure_file(
        SpelunkerConfig.h.in
        SpelunkerConfig.h
)

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
        ${CMAKE_CURRENT_BINARY_DIR}/SpelunkerConfig.h
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

add_library(spelunker SHARED
        ${PUBLIC_HEADER_FILES}
        ${PRIVATE_HEADER_FILES}
        ${SOURCE_FILES}
        )

# Install the lib, all public headers, and the processed SpelunkerConfig.h file.
# We cannot use install(DIRECTORY...) to do this since that would copy the CMakeLists.txt files as well.
install(TARGETS spelunker DESTINATION lib)
install(DIRECTORY DESTINATION include/spelunker)
foreach (s ${SUBDIRS})
    string(TOUPPER ${s} S)
    foreach(f ${${S}_PUBLIC_HEADER_FILES})
        install(FILES "${f}" DESTINATION include/spelunker/${s})
    endforeach()
endforeach()
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/SpelunkerConfig.h" DESTINATION include/spelunker)
