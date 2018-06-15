# CMakeLists.txt
# By Sebastian Raaphorst, 2018.

project(spelunker VERSION 0.1.0 LANGUAGES CXX)

configure_file(
        SpelunkerConfig.h.in
        SpelunkerConfig.h
)

set(SUBDIRS
        graphmaze
        math
        maze
        thickmaze
        typeclasses
        types
        )

# Add all files to the lists.
set(PUBLIC_HEADER_FILES "${CMAKE_CURRENT_BINARY_DIR}/SpelunkerConfig.h")
set(PRIVATE_HEADER_FILES)
set(SOURCE_FILES)
set(FILETYPES PUBLIC_HEADER_FILES PRIVATE_HEADER_FILES SOURCE_FILES)

foreach(pkg ${SUBDIRS})
    message("Adding ${pkg}")
    add_subdirectory(${pkg})

    # Now add the files from each to the above lists.
    string(TOUPPER ${pkg} PKG)
    foreach (ft ${FILETYPES})
        message("\tAppending ${ft} to ${${PKG}_${ft}}")
        list(APPEND ${ft} ${${PKG}_${ft}})
    endforeach()
endforeach()

# Create the shared library.
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
