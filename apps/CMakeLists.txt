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
        app_test_braid
        app_test_furthest_cells
        app_test_homomorphisms
        app_test_squishedmaze
        app_test_squishedthickmaze
        app_test_symmetry
        app_test_thickbraid
        app_test_thickify
        app_test_unicursal
        )

function(build_app appname)
    add_executable(${appname} ${appname}.cpp)
    target_link_libraries(${appname} LINK_PUBLIC spelunker)
endfunction()

foreach (app ${apps})
    build_app(${app})
endforeach()
