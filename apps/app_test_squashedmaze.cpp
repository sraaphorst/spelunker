/**
 * app_test_squishedmaze.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <iostream>

#include <boost/graph/adjacency_list.hpp>

#include <maze/DFSMazeGenerator.h>
#include <maze/Maze.h>
#include <maze/MazeTypeclasses.h>
#include <squashedmaze/SquashedMazeAttributes.h>
#include <squashedmaze/SquashedMaze.h>
#include <typeclasses/Show.h>

using namespace spelunker;
using namespace std;

int main() {
    constexpr auto width = 1000;
    constexpr auto height = 1000;
    const maze::DFSMazeGenerator dfs{width, height};
    const auto m = dfs.generate();
    cout << typeclasses::Show<maze::Maze>::show(m);


    const auto sm = squashedmaze::SquashedMaze(m);
    const auto &mp = sm.getMap();
    const auto &g  = sm.getGraph();

//    const auto vertices = g.vertex_set();
//    cout << "Number of vertices: " << vertices.size() << endl;
//    for (const auto v: vertices) {
//        const auto edges = g.out_edge_list(v);
//        cout << "Number of edges for vertex " << v << ": " << edges.size() << endl;
//    }
}