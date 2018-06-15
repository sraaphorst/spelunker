# CMakeLists.txt
# By Sebastian Raaphorst, 2018.

cmake_minimum_required(VERSION 3.10)
project(spelunker_top)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

# Find includes in corresponding build directories.
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

set (CMAKE_PREFIX_PATH "/Applications/Qt-5.9.4/5.9.4/clang_64" CACHE PATH "")
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)


find_package(Boost 1.31 REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
# Function to copy the current source dir to a list of filenames.
function(src_dir_filelist lst outlst)
    set(tmp "")
    foreach(f ${lst})
        list(APPEND tmp "${CMAKE_CURRENT_SOURCE_DIR}/${f}")
    endforeach()
    set(${outlst} "${tmp}" PARENT_SCOPE)
endfunction()


include_directories("${PROJECT_SOURCE_DIR}/src")
add_subdirectory(src)
#add_subdirectory(apps)
add_subdirectory(gui)


