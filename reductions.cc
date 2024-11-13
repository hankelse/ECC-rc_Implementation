

// #include <iostream>
// using namespace std;

// #include "graph.h"
// #include "clique.h"
// #include "node.h"
// #include "edge.h"

// /*
// size_t apply_rule_one(Graph const& graph, Cover& cover, size_t const component = 0) {
//     size_t ret = 0;

//     for (auto const& [v1, v1_neighbors] : graph.get_adj_list()) {
//         if (cover.is_removed(v1) or (component != 0 and cover.components[v1] != component)) continue;

//         bool all_edges_covered = true;
//         for (node_t v2 : v1_neighbors) {
//             if (cover.is_removed(v2)) continue;

//             if (not cover.is_covered(v1, v2)) {
//                 all_edges_covered = false;
//                 break;
//             }
//         }

//         if (all_edges_covered) {
//             cover.remove_node(v1);
//             // std::cout << "Rule 1 is removing " << v1 << "\n";
//             for (node_t neighbor : graph.neighbors(v1)) {
//                 cover.cover_edge(neighbor, v1);
//             }
//             ret++;
//         }
//     }

//     return ret;
// }

// */

// size_t apply_rule_one(Graph& G) {

// }


// /*
// size_t apply_rule_two(Graph const& graph, Cover& cover, size_t const component = 0) {
//     size_t ret = 0;

//     for (auto const& [v1, v1_neighbors] : graph.get_adj_list()) {
//         if (cover.is_removed(v1) or (component != 0 and cover.get_component(v1) != component)) continue;
//         for (node_t v2 : v1_neighbors) {
//             if (v2 <= v1 or cover.is_removed(v2) or cover.is_covered(v1, v2)) continue;

//             node_container_t common_neighbors;
//             compute_common_neighbors(graph, cover, v1, v2, common_neighbors);
//             common_neighbors.insert(v1);
//             common_neighbors.insert(v2);

//             if (is_clique(graph, cover, common_neighbors)) {
//                 cover.cover_clique(common_neighbors);
//                 // std::cout << "Rule 2 is adding the clique ";
//                 // for (auto const& n : common_neighbors) std::cout << n << " ";
//                 // std::cout << "based on edge " << v1 << " " << v2 << "\n";
//                 ret++;
//             }
//         }
//     }

//     return ret;
// }
// */

