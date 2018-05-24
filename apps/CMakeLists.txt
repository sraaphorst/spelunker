# Find includes in corresponding build directories.
set(CMAKE_INCLUDE_CURRENT_DIR  ON)

add_executable(aldous_broder aldous_broder.cpp Executor.h Utils.h)
target_link_libraries(aldous_broder LINK_PUBLIC spelunker)

add_executable(binarytree binarytree.cpp ProbabilisticExecutor.h Utils.h)
target_link_libraries(binarytree LINK_PUBLIC spelunker)

add_executable(bfs bfs.cpp Executor.h Utils.h)
target_link_libraries(bfs LINK_PUBLIC spelunker)

add_executable(dfs dfs.cpp Executor.h Utils.h)
target_link_libraries(dfs LINK_PUBLIC spelunker)

add_executable(eller eller.cpp Utils.h)
target_link_libraries(eller LINK_PUBLIC spelunker)

add_executable(growing_tree growing_tree.cpp Utils.h)
target_link_libraries(growing_tree LINK_PUBLIC spelunker)

add_executable(hunt_and_kill hunt_and_kill.cpp Executor.h Utils.h)
target_link_libraries(hunt_and_kill LINK_PUBLIC spelunker)

add_executable(kruskal kruskal.cpp Executor.h Utils.h)
target_link_libraries(kruskal LINK_PUBLIC spelunker)

add_executable(prim prim.cpp Executor.h Utils.h)
target_link_libraries(prim LINK_PUBLIC spelunker)

add_executable(prim2 prim2.cpp Executor.h Utils.h)
target_link_libraries(prim2 LINK_PUBLIC spelunker)

add_executable(recursive_division recursive_division.cpp Utils.h)
target_link_libraries(recursive_division LINK_PUBLIC spelunker)

add_executable(sidewinder sidewinder.cpp ProbabilisticExecutor.h Utils.h)
target_link_libraries(sidewinder LINK_PUBLIC spelunker)

add_executable(wilson wilson.cpp Executor.h Utils.h)
target_link_libraries(wilson LINK_PUBLIC spelunker)

add_executable(thicktest testthick.cpp)
target_link_libraries(thicktest LINK_PUBLIC spelunker)