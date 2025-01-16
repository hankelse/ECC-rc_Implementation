#include "ecc-fr1.h"



#include "graph.h"
#include "node.h"
#include "edge.h"
#include "clique.h"
#include "fast_set.h"
#include "io.h"
#include "tools.h"

// #ifdef PROFILING
      
// #endif 
#include <chrono>
// #include <gperftools/profiler.h> 



ECC_FR1::ECC_FR1(string ds_filepath) : ECC_NEC (ds_filepath) {
    // common_neighbor_sets = vector<vector<Node*>>(G->_edges.size(), vector<Node*>(0));

    // common_neighbor_sets.reserve(G->_edges.size());
    common_neighbor_sets = vector<vector<Node*>>(G->_edges.size(), {nullptr});
    edge_removals = vector<bool>(G->_edges.size(), false);
    edge_4_clique_memberships = vector<size_t>(G->_edges.size(), 0);


}

ECC_FR1::ECC_FR1(Graph& G) : ECC_NEC (G) {
    name = "ECC_FR1";
    // common_neighbor_sets = vector<vector<Node*>>(G._edges.size(), vector<Node*>(0));
    edge_4_clique_memberships = vector<size_t>(G._edges.size(), 0);
    common_neighbor_sets = vector<vector<Node*>>(G._edges.size(), {nullptr});
    edge_removals = vector<bool>(G._edges.size(), false);

}

/**
 * @brief ECC_FR1 applies exhaustive reductions, fills common_neighbor_sets and edge_4_clique_memberships, finds an ordering for the algorithm, and then runs the framework
 * 
 *
 * @return a clique cover of G
 */
vector<Clique*>* ECC_FR1::run() {
    // cout << "RUNNING FR!" << endl;
    init_reduction_data();

    // for (int i = 0; i < G->_edges.size(); i++) {
    //     cout << *G->_edges[i] << " : " << edge_4_clique_memberships[i] << endl;
    // }
    // cout << edge_4_clique_memberships << endl;
    //applies reductions
    // apply_rules_exhaustively();
    apply_rule_two();
    apply_rule_one();
    // cout << "\t\t\t(reduced data: -" << num_edges_covered << " edges and -" << nodes_removed << " nodes.)" << endl;

    // for (int i = 0; i < G->_edges.size(); i++) {
    //     cout << *G->_edges[i] << " : " << edge_removed(G->_edges[i]) << endl;
    // }

    //Fill common_neighbor_sets and edge_4_clique_memberships
    get_edge_ordering();
    
    return ECC::run();
}



size_t ECC_FR1::apply_rule_one() {
    // cout << "started r1 " << endl;
    size_t starting_num_removed = nodes_removed;
    
    for (Node* node : G->_nodes) {
        
        if (is_removed(node)) continue; // Skip node if removed

        //check if all incident edges have been covered
        vector<Edge*> incident_edges = node->edges;
        bool all_covered = true;
        for (Edge* edge : incident_edges) {
            if (edge_removed(edge)) continue;
            if (!edge->_covered) {
                all_covered = false;
                break;
            }
        }

        //if all incident edges covered, remove node from G
        if (all_covered) {
            //remove connecting node
            remove_node(node);

            //remove incident edges, and update the corresponding counts in edge_4_clique_memberships
            for (Edge* incident_edge : incident_edges) {
                if (edge_removed(incident_edge)) continue;
                remove_edge(incident_edge);
            }


            // Adjust edge_4_clique_memberships
                // for each edge covered, "we remove them" -> must adjust edge counts wherever edge is included


            //Adjust common neighbor lists
                // for each edge the node is in, if the node is in its common neighbor list, remove it


        }
    }
    // cout << "ended r1 " << endl;
    return nodes_removed - starting_num_removed;
}

size_t ECC_FR1::apply_rule_two() {
    // cout << "started r2" << endl;
    size_t cliques_found = 0;

    Clique* found_clique;
    for (Node* node : G->_nodes) {
        // cout << "node: " << *node << endl;
        if (is_removed(node)) continue; // skip node if already removed


        for (const auto & [neighbor, connecting_edge] : node->connection_map) {

            if (is_removed(neighbor)) continue; // skip neighbor if already removed
            if (connecting_edge->is_covered()) continue; //skip neighbor if connecting edge already covered.
            if (edge_removed(connecting_edge)) continue; //skip if edge removed (shouldn't trigger is prev passed)

            // count # common neighbors not removed
            vector<Node*> common_neighbors = get_common_neighbors(connecting_edge);
            size_t num_common_neighbors = 0;
            for (Node* node : common_neighbors) {
                if (is_removed(node)) {continue;};
                num_common_neighbors += 1;
            }

            size_t expected_num_4_cliques = (num_common_neighbors * (num_common_neighbors -1))/2;
            // cout << "Can we remove " << *connecting_edge << "?" << endl;
            // cout << " \t expected_num_4_cliques = " << expected_num_4_cliques << endl;
            // cout << " \t num_common_neighbors = " << num_common_neighbors << endl;
            // cout << " \t 4-clique-memberships = " << edge_4_clique_memberships[connecting_edge->index] << endl;
            // cout << "\t common neighbors = " << common_neighbors << endl;

            if (expected_num_4_cliques == edge_4_clique_memberships[connecting_edge->index]) {

                // cout << "got match!" << endl;
                vector<Node*> common_neighbors = get_common_neighbors(connecting_edge);
                common_neighbors.push_back(node);
                common_neighbors.push_back(neighbor);

                found_clique = new Clique();
                for (Node* node : common_neighbors) {
                    if (is_removed(node)) continue;
                    add_to_clique(found_clique, node);
                }
                fill_clique(found_clique);
                clique_cover.push_back(found_clique);
                cliques_found++;
            }
        }
    }
    // cout << "ended r2" << endl;
    return cliques_found;
}


// vector<Node*> ECC_FR1::get_degeneracy_ordering(vector<size_t>& degeneracy_indices) {
//     vector<Node*> degen_ordering; 

//     vector<size_t> node_degrees;
//     node_degrees.reserve(G->_nodes.size());

//     Node* minimum_degree_node = G->_nodes[0]; 
//     size_t minimum_degree = G->_nodes[0]->neighbors.size();


//     // Initialize node_degrees, find first min
//     for (Node* node : G->_nodes) {
//         size_t node_degree = node->neighbors.size();
//         node_degrees.emplace_back(node_degree);

//         // if smallest, update current
//         if (node_degree < minimum_degree) {
//             minimum_degree = node_degree;
//             minimum_degree_node = node;
//         }
//     }

//     // "remove" min degree node and put into ordering
//     node_degrees[minimum_degree_node->index] = -1; 
//     for (Node* neighbor : minimum_degree_node->neighbors) {
//         if (node_degrees[neighbor->index] == -1) continue; // skip if neighbor "removed"
//         node_degrees[neighbor->index] -= 1;
//     }
//     // Save the index of the node in the ordering
//     degeneracy_indices[minimum_degree_node->index] = degen_ordering.size();
//     degen_ordering.push_back(minimum_degree_node);
    


//     //repeat until all nodes in ordering
//     while (degen_ordering.size() < G->_nodes.size()) {

//         minimum_degree_node = nullptr;
//         minimum_degree = G->_nodes.size();

//         //find the smallest degree node
//         for (size_t i = 0; i < node_degrees.size(); i++) {
//             if (node_degrees[i] == -1) continue; // skip if node "removed"
            
//             //update local minimum
//             if (node_degrees[i] < minimum_degree) {
//                 minimum_degree = node_degrees[i];
//                 minimum_degree_node = G->_nodes[i];
//             }
//         }

//         //"remove" minimum and update neighbors
//         node_degrees[minimum_degree_node->index] = -1;
//         for (Node* neighbor : minimum_degree_node->neighbors) {
//             if (node_degrees[neighbor->index] == -1) continue; // skip if neighbor "removed"
//             node_degrees[neighbor->index] -= 1;
//         }  
//         // Save the index of the node in the ordering
//         degeneracy_indices[minimum_degree_node->index] = degen_ordering.size();
//         // Add node to ordering
//         degen_ordering.push_back(minimum_degree_node); 
//     }
//     return degen_ordering;
// }

/**
 * @brief Makes edge_ordering a list of edges sorted by the values in edge-intersection-counts
 * 
 */
void ECC_FR1::get_edge_ordering() {

    // Copy all edges to edge_ordering
    edge_ordering = G->_edges; 
    const Graph* G = this->G;
    vector<size_t>& edge_counts = edge_4_clique_memberships;

    // Sort edge_ordering based on edge_intersection_counts from greatest to least
    sort(edge_ordering.begin(), edge_ordering.end(),
        [&edge_counts, &G](Edge* e1, Edge* e2) {
            // Find indices of edges a and b in G->_edges
            size_t e1_index = e1->index;
            size_t e2_index = e2->index;

            // Sort by counts in descending order
            // return edge_counts[e1_index] > edge_counts[e2_index];

            //Sort by counts in ascending order
            return edge_counts[e1_index] < edge_counts[e2_index];
        });
}

vector<Node*> ECC_FR1::get_degeneracy_ordering(vector<size_t>& degeneracy_indices) {
    // Map to track the degree of each node
    std::unordered_map<Node*, int> degree;
    for (std::vector<Node*>::const_iterator it = G->_nodes.begin(); it != G->_nodes.end(); ++it) {
        Node* node = *it;
        degree[node] = node->neighbors.size();
    }

    // Custom comparator for priority queue
    struct Compare {
        std::unordered_map<Node*, int>& degree;
        Compare(std::unordered_map<Node*, int>& degreeMap) : degree(degreeMap) {}
        bool operator()(Node* a, Node* b) const {
            return degree[a] > degree[b];
        }
    };

    // Priority queue for nodes sorted by degree (min-heap)
    Compare comp(degree);
    std::priority_queue<Node*, std::vector<Node*>, Compare> pq(comp);

    // Add all nodes to the priority queue
    for (std::vector<Node*>::const_iterator it = G->_nodes.begin(); it != G->_nodes.end(); ++it) {
        pq.push(*it);
    }

    std::vector<Node*> ordering; // Resulting degeneracy ordering
    std::unordered_set<Node*> processed; // Set of already processed nodes

    while (!pq.empty()) {
        Node* current = pq.top();
        pq.pop();

        // Skip nodes that have already been processed
        if (processed.find(current) != processed.end()) {
            continue;
        }

        // Add to ordering and mark as processed
        degeneracy_indices[current->index] = ordering.size();
        ordering.push_back(current);
        processed.insert(current);

        // Reduce the degree of neighbors and update the priority queue
        for (std::vector<Node*>::iterator it = current->neighbors.begin();
             it != current->neighbors.end(); ++it) {
            Node* neighbor = *it;
            if (processed.find(neighbor) == processed.end()) {
                degree[neighbor]--;
                pq.push(neighbor); // Update the priority queue with the new degree
            }
        }
    }

    return ordering;
}

void ECC_FR1::remove_node(Node* node) {
    node_removals[node->index] = true;
    nodes_removed++;
    
    // // remove node from each of the node's neighbor's neighbor's
    // for (Node* neighbor : node->neighbors) {
    //     vector<Node*>& neighbors2  = neighbor->neighbors;

    //     auto it = find(neighbors2.begin(), neighbors2.end(), node);
    //     swap(*it, neighbors2.back());
    //     neighbors2.pop_back();
    // }

    // //remove from all neighbors connection-maps
    // for (Node* neighbor : node->neighbors) {
    //     neighbor->connection_map.erase(node);
    // }

    // remove from all common neighbors??

}

void ECC_FR1::remove_edge(Edge* edge) {
    edge_removals[edge->index] = true;

    //update edge_4_clique_memberships of effected edges
    for (Edge* referencing_edge : edge->counted_by) {
        edge_4_clique_memberships[referencing_edge->index] -= 1;
    }

    // //remove edge from all of n1, n2 edges (one should already be removed by rule1)
    // Node* n1 = edge->_node1;
    // Node* n2 = edge->_node2;

    // // remove using swap and pop method 
    // if (not is_removed(n1)) {
    //     vector<Edge*>& n1_edges = n1->edges;
    //     auto it = find(n1_edges.begin(), n1_edges.end(), edge);
    //     swap(*it, n1_edges.back());
    //     n1_edges.pop_back();
    // }

    // if (not is_removed(n2)) {
    //     vector<Edge*>& n2_edges = n2->edges;
    //     auto it = find(n2_edges.begin(), n2_edges.end(), edge);
    //     swap(*it, n2_edges.back());
    //     n2_edges.pop_back();
    // }




}


bool ECC_FR1::edge_removed(Edge* edge) {
    return edge_removals[edge->index];
}


/**
 * @brief Finds a clique from an edge using Conte's framework and the pre-computed common_neighbors
 * 
 *
 * @param edge 
 * @return 
 */
Clique* ECC_FR1::find_clique_of(Edge* edge) {

    //R ← {u, v}  create the new clique with the edge
    Clique* clique = new Clique();
    clique->index = clique_cover.size();  // New to QEC: adds the index of the clique 

    Node* u = edge->_node1;
    Node* v = edge->_node2;

    if (is_removed(u) || is_removed(v)) {
        throw invalid_argument("Passed edge with removed node(s)");
    }

    // cout << "finding clique of " << *edge << endl;

    add_to_clique(clique, u);
    add_to_clique(clique, v);

    //P ← N(u)∩ N(v)
    vector<Node*> candidates = get_common_neighbors(edge);
    // cout << "candidates" << candidates << endl;

    // z ← EXTRACT NODE(P)
    
    Node* new_member = extract_node(candidates, clique);
    // cout << "new member: " << new_member << endl;

    // while there are more potential additions:
    while (new_member != nullptr) {
        // cout << "looping!" << endl;

        //add new_node to clique (COVERS EDGES)
        add_to_clique(clique, new_member);

        // Trim candidates: P ← P ∩ N(z), sped up using the common_neighbors
        Edge* new_edge_with_u = G->get_edge(u, new_member);
        // trim_candidates(candidates, new_member->neighbors);


        trim_candidates(candidates, get_common_neighbors(new_edge_with_u));

        // Extract next node
        new_member = extract_node(candidates, clique);

    }
    // cout << "time to fille" << endl;
    fill_clique(clique);
    return clique;
}


/**
 * @brief Iterates through the edges in G. For e = edges[i], 
 * saves its common neighbors in common_neighbor_sets[i]
 * and the number of edges between common neighbors in edge_4_clique_memberships
 * 
 */
/** 
void ECC_FR1::init_reduction_data() {
    // chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();

    vector<size_t> degeneracy_indices(G->_nodes.size(), -1);

    // Gets a degeneracy ordering, saves the index of each node in degen indices
    vector<Node*> degeneracy_ordering = get_degeneracy_ordering(degeneracy_indices); // filles degen_indexes with indexs in degen ordering


    vector<bool> edge_checked(G->_edges.size(), false);


    // Look at each node in the degeneracy_ordering
    for (size_t degen_index = 0; degen_index < degeneracy_ordering.size(); degen_index++) {
        // chrono::steady_clock::time_point end_time = chrono::steady_clock::now();
        // size_t runtime = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
        // if (runtime > 5000) {
        //     // return;
        //     ProfilerStop();
        //     exit(0);
        // }

        Node* node = degeneracy_ordering[degen_index];

        cout << " " << degen_index << " / " << degeneracy_ordering.size() << endl;

        // Get the neighbor, edge pairs in a vector
        vector<pair<Node*, Edge*>> ordered_neighbors = {};
        for (const auto & [neighbor, edge] : node->connection_map) {
            ordered_neighbors.push_back(pair(neighbor, edge));
        }


        //Sort neighbors by the degen index of the neighbor
        sort(ordered_neighbors.begin(), ordered_neighbors.end(),
              [degeneracy_indices](const pair<Node*, Edge*>& neighbor1, const pair<Node*, Edge*>& neighbor2) {
                size_t neighbor1_degen_index = degeneracy_indices[neighbor1.first->index];
                size_t neighbor2_degen_index = degeneracy_indices[neighbor2.first->index];
                return neighbor1_degen_index < neighbor2_degen_index;
              });

        // cout << "\t size: " << ordered_neighbors.size() << endl;
        for (pair<Node*, Edge*>& pair : ordered_neighbors) {
            // cout << "\t\t a" << endl;

            Node* neighbor = pair.first;
            Edge* edge = pair.second;

            //Skip over edges if already seen
            if (edge_checked[edge->index]) continue;
            else edge_checked[edge->index] = true;


            vector<Node*> common_neighbors = node_set_intersect(node->neighbors, neighbor->neighbors);

            common_neighbor_sets[edge->index] = std::move(common_neighbors);

            // trim to later common neighbors
            vector<Node*> later_common_neighbors = {};
            size_t node_degen_index = degeneracy_indices[node->index];
            size_t neighbor_degen_index = degeneracy_indices[neighbor->index];
            size_t edge_degree = max(node_degen_index, neighbor_degen_index);
            for (Node* common_neighbor : common_neighbor_sets[edge->index]) {
                size_t common_neighbor_degen_index = degeneracy_indices[common_neighbor->index];
                if (common_neighbor_degen_index > edge_degree) {
                    later_common_neighbors.push_back(common_neighbor);
                }
            }

            cout << later_common_neighbors.size() << endl;
            update_4_clique_counts(edge, later_common_neighbors);
            
        }
    }
   
    size_t sum = 0;
    for (size_t number : edge_4_clique_memberships) {
        sum += number;
    }
    // cout << "a total of " << sum << " 4-clique memberships" << endl;
}
*/


void init_later_neighbors(vector<Node*>& degen_ordering, vector<size_t>& degen_indices) {
    for (size_t node_degen_index = 0; node_degen_index < degen_ordering.size(); node_degen_index++) {
        Node* node = degen_ordering[node_degen_index];
        for (Node* neighbor : node->neighbors) {
            if (degen_indices[neighbor->index] > node_degen_index) {
                node->later_neighbors.push_back(neighbor);
            }
        }
        // Sort later neighbors
        sort(node->later_neighbors.begin(), node->later_neighbors.end(), [&degen_indices](Node* node1, Node* node2) {
        return degen_indices[node1->index] < degen_indices[node2->index];
        });
    }
}

void ECC_FR1::init_reduction_data() {
    /** 
    cout << "[13648] is not connected to Node [22838]" << endl;
    Node* node1 = nullptr;
    Node* node2 = nullptr;
    size_t i = 0;
    while (node1 == nullptr || node2 == nullptr) {
        if (G->_nodes[i]->id() == 13648) node1 = G->_nodes[i];

        if (G->_nodes[i]->id() == 22838) node2 = G->_nodes[i];

        i++;
    }
    cout << "RESULT: " << G->get_edge(node1, node2) << endl;
    */

    // Initialize common neighbors
    for (Edge* edge : G->_edges) {
        common_neighbor_sets[edge->index] = node_set_intersect(edge->_node1->neighbors, edge->_node2->neighbors);
    }

    // Get degeneracy ordering
    vector<size_t> degeneracy_indices(G->_nodes.size(), -1);
    vector<Node*> degen_ordering = get_degeneracy_ordering(degeneracy_indices);

    // Initialize later neighbors
    init_later_neighbors(degen_ordering, degeneracy_indices);
    // for (size_t node_degen_index = 0; node_degen_index < degen_ordering.size(); node_degen_index++) {
    //     Node* node = degen_ordering[node_degen_index];
    //     for (Node* neighbor : node->neighbors) {
    //         if (degeneracy_indices[neighbor->index] > node_degen_index) {
    //             node->later_neighbors.push_back(neighbor);
    //         }
    //     }
    //     // Sort later neighbors
    //     sort(node->later_neighbors.begin(), node->later_neighbors.end(), [&degeneracy_indices](Node* node1, Node* node2) {
    //     return degeneracy_indices[node1->index] < degeneracy_indices[node2->index];
    //     });
    // }

    // For node in the degeneracy ordering
    for (size_t node_degen_index = 0; node_degen_index < degen_ordering.size(); node_degen_index++) {
        Node* node = degen_ordering[node_degen_index];


        // Iterate through later neighbors (sorted by degen)
        for (Node* neighbor : node->later_neighbors) {
            Edge* edge = G->get_edge(node, neighbor);


            vector<Node*> later_common_neighbors = node_set_intersect(neighbor->later_neighbors, node->later_neighbors);

            //Iterate through every pair of common_neighbors:

            for (size_t ti = 0; ti < later_common_neighbors.size(); ti++) {
                Node* tri_node = later_common_neighbors[ti];

                for (size_t qi = ti+1; qi < later_common_neighbors.size(); qi++) {
                    Node* quad_node = later_common_neighbors[qi];

                    Edge* triangle_connection = G->get_edge(tri_node, quad_node);

                    if (triangle_connection != nullptr) {
                        
                        // Get 4 implied edges (could be done before)

                        Edge* tri_to_node = G->get_edge(node, tri_node);
                        Edge* quad_to_node = G->get_edge(node, quad_node);
                        Edge* tri_to_neigh = G->get_edge(neighbor, tri_node);
                        Edge* quad_to_neigh = G->get_edge(neighbor, quad_node);

                        //Update all counts
                        // cout << "ADJUSTING";
                        for (Edge* clique_edge : {edge, triangle_connection, tri_to_node, quad_to_node, tri_to_neigh, quad_to_neigh}) {
                            edge_4_clique_memberships[clique_edge->index] += 1;
                            
                            // cout << " " << *clique_edge << " ";

                        }
                        // cout << endl;
                        

                    }

                }
                
            }




        }


    }

    // for (int i = 0; i < G->_edges.size(); i++) {
    //     cout << *G->_edges[i] << " : " << edge_4_clique_memberships[i] << endl;
    // }
}

void ECC_FR1::update_4_clique_counts(Edge* edge, vector<Node*>& later_com_neighbors) {
    // cout << "update-4-clique-counts: \n\tedge=" << *edge << " \n\tnum later_com_neighbors = " << later_com_neighbors.size() << endl;
    // Get the current number of 4 cliques edge is apart of (could have been incremented by previous edge)
    size_t curr_count = edge_4_clique_memberships[edge->index];



    for (int n1 = 0; n1 < later_com_neighbors.size(); n1++) {
        Node* node1 = later_com_neighbors[n1];
        if (is_removed(node1)) continue; // skip if node removed

        for (int n2 = n1 + 1; n2 < later_com_neighbors.size(); n2++) {
            Node* node2 = later_com_neighbors[n2];
            if (is_removed(node2)) continue; // skip if node removed

            // check for edge between triangles
            Edge* tri_connection = G->get_edge(node1, node2);
            // cout << "tri_connection address = " << tri_connection << endl;
            if (tri_connection != nullptr) {
                // vector<Edge*> clique_edges = vector<Edge*>(6, nullptr);
                vector<Edge*> clique_edges;
                clique_edges.reserve(6);

                // cout << "tri_connection address = " << tri_connection << endl;

                //Update count
                curr_count += 1;
                clique_edges.emplace_back(edge);

                // Update opposite edge's count
                edge_4_clique_memberships[tri_connection->index] += 1;
                clique_edges.emplace_back(tri_connection);

                //Get the other edges and update
                for (Node* tri_node : {node1, node2}) {
                    for (Node* edge_node : {edge->_node1, edge->_node2}) {
                        Edge* connection = G->get_edge(tri_node, edge_node);
                        edge_4_clique_memberships[connection->index] += 1;
                        clique_edges.emplace_back(connection);
                    }
                }

                // add all edges to eachother's "counted by"
                // cout << "clique edges: " << clique_edges << endl;
                for (Edge* edge1 : clique_edges) {
                    for (Edge* edge2 : clique_edges) {
                        if (edge1->index != edge2->index) {
                            edge1->counted_by.push_back(edge2);
                        }
                    }
                }

            }

        }
    } 
    edge_4_clique_memberships[edge->index] = curr_count; 
    // cout << "\t --> update-4-cliques-done" << endl;
}

/**
 * @brief Returns the common neighbors of the nodes in the edge
 * 
 *
 * @param edge 
 * @return vector of common neighbors
 */
vector<Node*> ECC_FR1::get_common_neighbors(Edge* edge) {
    vector<Node*>& candidates = common_neighbor_sets[edge->index];
    // vector<Node*> temp_candidates = candidates;

    // vector<Node*> result;
    // result.reserve(candidates.size());
    // // Remove nodes that were removed.
    // for (Node* node : temp_candidates) {
    //     if (is_removed(node)) {
    //         // swap and pop to remove node
    //         auto it = find(candidates.begin(), candidates.end(), node);
    //         swap(*it, candidates.back());
    //         candidates.pop_back();


    //         continue;
    //     }
    //     result.emplace_back(node);
    // }

    return candidates;
}

