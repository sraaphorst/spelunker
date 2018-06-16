# CMakeLists.txt
# By Sebastian Raaphorst, 2018.

set(apps
        aldous_broder
        binarytree
        bfs
        cellular_automaton
        dfs
        eller
        grid_colouring
        growing_tree
        hunt_and_kill
        kruskal
        prim
        prim2
        recursive_division
        sidewinder
        wilson
        test_braid
        test_homomorphisms
        test_thickbraid
        test_thickify
        test_unicursal
        )

function(build_app appname)
    add_executable(${appname} ${appname}.cpp)
    target_link_libraries(${appname} LINK_PUBLIC spelunker)
endfunction()

foreach (app ${apps})
    build_app(${app})
endforeach()
