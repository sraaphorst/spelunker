# CMakeLists.txt
# By Sebastian Raaphorst, 2018.

# Find includes in corresponding build directories.
set(CMAKE_INCLUDE_CURRENT_DIR  ON)

add_executable(aldous_broder aldous_broder.cpp Executor.h Utils.h)
target_link_libraries(aldous_broder LINK_PUBLIC spelunker)

add_executable(binarytree binarytree.cpp ProbabilisticExecutor.h Utils.h)
target_link_libraries(binarytree LINK_PUBLIC spelunker)

add_executable(bfs bfs.cpp Executor.h Utils.h)
target_link_libraries(bfs LINK_PUBLIC spelunker)

add_executable(cellular_automaton cellular_automaton.cpp)
target_link_libraries(cellular_automaton LINK_PUBLIC spelunker)

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

add_executable(test_braid test_braid.cpp)
target_link_libraries(test_braid LINK_PUBLIC spelunker)

add_executable(test_homomorphisms test_homomorphisms.cpp)
target_link_libraries(test_homomorphisms LINK_PUBLIC spelunker)

add_executable(test_thickbraid test_thickbraid.cpp)
target_link_libraries(test_thickbraid LINK_PUBLIC spelunker)

add_executable(test_thickify test_thickify.cpp)
target_link_libraries(test_thickify LINK_PUBLIC spelunker)

add_executable(test_unicursal test_unicursal.cpp)
target_link_libraries(test_unicursal LINK_PUBLIC spelunker)

add_executable(grid_colouring grid_colouring.cpp)
target_link_libraries(grid_colouring LINK_PUBLIC spelunker)
