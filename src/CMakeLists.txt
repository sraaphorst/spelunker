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

# Function to take a package, and then prepend the path of the package to the
# list of files in the PARENT_SCOPE:
# 1. _PKGNAME_PUBLIC_HEADER_FILES
# 2. _PKGNAME_PRIVATE_HEADER_FILES
# 3. _PKGNAME_SOURCE_FILES
# These are then exported, with qualified paths, as PARENT_SCOPE:
# 1. PKGNAME_PUBLIC_HEADER_FILES
# 2. PKGNAME_PRIVATE_HEADER_FILES
# 3. PKGNAME_SOURCE_FILES
function(sourcedir_package pkgname)
    string(TOUPPER ${pkgname} PKGNAME)

    foreach(filetype ${FILETYPES})
        set(tmp "")
        foreach(filename ${_${PKGNAME}_${filetype}})
            list(APPEND tmp "${CMAKE_CURRENT_SOURCE_DIR}/${pkgname}/${filename}")
        endforeach()
        set(${PKGNAME}_${filetype} "${tmp}" PARENT_SCOPE)
    endforeach()
endfunction()


# Iterate over all the different packages, find their qualified paths, and collect
# them up into lists:
# 1. PUBLIC_HEADER_FILES
# 2. PRIVATE_HEADER_FILES
# 3. SOURCE_FILES
foreach(pkgname ${SUBDIRS})
    add_subdirectory(${pkgname})
    sourcedir_package(${pkgname})

    # Now add the files from each to the above lists.
    string(TOUPPER ${pkgname} PKGNAME)
    foreach (filetype ${FILETYPES})
        list(APPEND ${filetype} ${${PKGNAME}_${filetype}})
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
