#include "ecc-fr.h"



#include "graph.h"
#include "node.h"
#include "edge.h"
#include "clique.h"
#include "fast_set.h"
#include "io.h"



ECC_FR::ECC_FR(string ds_filepath) : ECC_NEC (ds_filepath) {
    // common_neighbor_sets = vector<vector<Node*>>(G->_edges.size(), vector<Node*>(0));

    common_neighbor_sets.reserve(G->_edges.size());
    edge_removals = vector<bool>(G->_edges.size(), false);


}

ECC_FR::ECC_FR(Graph& G) : ECC_NEC (G) {
    name = "ECC_FR";
    // common_neighbor_sets = vector<vector<Node*>>(G._edges.size(), vector<Node*>(0));
    common_neighbor_sets.reserve(G._edges.size());
    edge_removals = vector<bool>(G._edges.size(), false);

}

/**
 * @brief ECC_FR applies exhaustive reductions, fills common_neighbor_sets and edge_intersection_counts, finds an ordering for the algorithm, and then runs the framework
 * 
 *
 * @return a clique cover of G
 */
vector<Clique*>* ECC_FR::run() {
    ECC_NEC::apply_rule_two();
    ECC_NEC::apply_rule_one();

    init_reduction_data();

    //applies reductions
    apply_rules_exhaustively();

    //Fill common_neighbor_sets and edge_intersection_counts
    get_edge_ordering();

    // exit(1);

    return ECC::run();
}


size_t ECC_FR::apply_rule_two() {
    size_t cliques_found = 0;

    Clique* found_clique;
    for (Node* node : G->_nodes) {
        if (node_removals[node->index]) continue; // skip node if already removed

        for (const auto & [neighbor, connecting_edge] : node->connection_map) {
            if (node_removals[neighbor->index]) continue; // skip neighbor if already removed
            if (connecting_edge->is_covered()) continue; //skip neighbor if connecting edge already covered.
            if (edge_removed(connecting_edge)) continue; //skip if edge removed (shouldn't trigger is prev passed)


            size_t num_common_neighbors = get_common_neighbors(connecting_edge).size() + 2; // add the two nodes from connceting edge

            size_t expected_num_edges = (num_common_neighbors * (num_common_neighbors -1))/2;

            if (expected_num_edges == edge_intersection_counts[connecting_edge->index]) {

                vector<Node*> common_neighbors = get_common_neighbors(connecting_edge);
                common_neighbors.push_back(node);
                common_neighbors.push_back(neighbor);

                found_clique = new Clique();
                for (Node* node : common_neighbors) {
                    add_to_clique(found_clique, node);
                }
                fill_clique(found_clique);
                clique_cover.push_back(found_clique);
                cliques_found++;
            }
        }
    }
    return cliques_found;
}


void ECC_FR::remove_node(Node* node) {
    node_removals[node->index] = true;
    nodes_removed++;
    
    // remove node from each of the node's neighbor's neighbor's
    for (Node* neighbor : node->neighbors) {
        vector<Node*>& neighbors2  = neighbor->neighbors;

        auto it = find(neighbors2.begin(), neighbors2.end(), node);
        swap(*it, neighbors2.back());
        neighbors2.pop_back();
    }

    //remove from all neighbors connection-maps
    for (Node* neighbor : node->neighbors) {
        neighbor->connection_map.erase(node);
    }

    // remove from all common neighbors??

}

void ECC_FR::remove_edge(Edge* edge) {
    edge_removals[edge->index] = true;

    //update edge_intersection_counts of effected edges
    for (Edge* referencing_edge : edge->counted_by) {
        edge_intersection_counts[referencing_edge->index] -= 1;
    }

    //remove edge from all of n1, n2 edges (one should already be removed by rule1)
    Node* n1 = edge->_node1;
    Node* n2 = edge->_node2;

    // remove using swap and pop method 
    if (not is_removed(n1)) {
        vector<Edge*>& n1_edges = n1->edges;
        auto it = find(n1_edges.begin(), n1_edges.end(), edge);
        swap(*it, n1_edges.back());
        n1_edges.pop_back();
    }

    if (not is_removed(n2)) {
        vector<Edge*>& n2_edges = n2->edges;
        auto it = find(n2_edges.begin(), n2_edges.end(), edge);
        swap(*it, n2_edges.back());
        n2_edges.pop_back();
    }




}


bool ECC_FR::edge_removed(Edge* edge) {
    return edge_removals[edge->index];
}

size_t ECC_FR::apply_rule_one() {
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

            //remove incident edges, and update the corresponding counts in edge_intersection_counts
            for (Edge* incident_edge : incident_edges) {
                if (edge_removed(incident_edge)) continue;
                remove_edge(incident_edge);
            }


            // Adjust edge_intersection_counts
                // for each edge covered, "we remove them" -> must adjust edge counts wherever edge is included


            //Adjust common neighbor lists
                // for each edge the node is in, if the node is in its common neighbor list, remove it


        }
    }

    return nodes_removed - starting_num_removed;
}


/**
 * @brief Finds a clique from an edge using Conte's framework and the pre-computed common_neighbors
 * 
 *
 * @param edge 
 * @return 
 */
Clique* ECC_FR::find_clique_of(Edge* edge) {

    //R ← {u, v}  create the new clique with the edge
    Clique* clique = new Clique();
    clique->index = clique_cover.size();  // New to QEC: adds the index of the clique 

    Node* u = edge->_node1;
    Node* v = edge->_node2;

    if (is_removed(u) || is_removed(v)) {
        throw invalid_argument("Passed edge with removed node(s)");
    }

    add_to_clique(clique, u);
    add_to_clique(clique, v);

    //P ← N(u)∩ N(v)
    vector<Node*> candidates = get_common_neighbors(edge);

    // z ← EXTRACT NODE(P)
    Node* new_member = extract_node(candidates, clique);

    // while there are more potential additions:
    while (new_member != nullptr) {

        //add new_node to clique (COVERS EDGES)
        add_to_clique(clique, new_member);

        // Trim candidates: P ← P ∩ N(z), sped up using the common_neighbors
        Edge* new_edge_with_u = G->get_edge(u, new_member);
        // trim_candidates(candidates, new_member->neighbors);
        trim_candidates(candidates, get_common_neighbors(new_edge_with_u));

        // Extract next node
        new_member = extract_node(candidates, clique);

    }
    fill_clique(clique);
    return clique;
}

/**
 * @brief Iterates through the edges in G. For e = edges[i], 
 * saves its common neighbors in common_neighbor_sets[i]
 * and the number of edges between common neighbors in edge_intersection_counts
 * 
 */
void ECC_FR::init_reduction_data() {
    for (Edge* edge : G->_edges) {
        if (edge_removed(edge)) continue;

        // Get each node in the edge
        Node* n1 = edge->_node1;
        Node* n2 = edge->_node2;

        // Look at the intersection

        vector<Node*> common_neighbors = node_set_intersect(n1->neighbors, n2->neighbors);

        // Add common_neighbors to common_neighbor_sets
        common_neighbor_sets.emplace_back(std::move(common_neighbors));


        // add n1 and n2 for calculations
        vector<Node*> neighborhood = common_neighbor_sets.back();
        neighborhood.reserve(neighborhood.size() + 2);
        neighborhood.emplace_back(n1);
        neighborhood.emplace_back(n2);

        size_t edge_count = get_edge_intersection_count(edge, neighborhood);
        
        //Put the number of edges between the common neighbors into the list
        edge_intersection_counts[edge->index] = edge_count;
    }

}



size_t ECC_FR::get_edge_intersection_count(Edge* edge, vector<Node*>& neighborhood) {
    // Calculate the edges between common neighbors
    size_t edge_count = 0;
    for (int n1_index = 0; n1_index < neighborhood.size(); n1_index++) {
        for (int n2_index = n1_index + 1; n2_index < neighborhood.size(); n2_index++ ) {

            //is there an edge here?
            Edge* connecting_edge = G->get_edge(neighborhood[n1_index], neighborhood[n2_index]);
            if (connecting_edge != nullptr) {
                edge_count += 1;

                //add reference to connecting_edge:
                connecting_edge->counted_by.push_back(edge);
            }
        }
    }
    return edge_count;
}

/**
 * @brief Returns the common neighbors of the nodes in the edge
 * 
 *
 * @param edge 
 * @return vector of common neighbors
 */
vector<Node*> ECC_FR::get_common_neighbors(Edge* edge) {
    vector<Node*>& candidates = common_neighbor_sets[edge->index];
    vector<Node*> temp_candidates = candidates;

    vector<Node*> result;
    result.reserve(candidates.size());
    for (Node* node : temp_candidates) {
        if (is_removed(node)) {
            // swap and pop to remove node
            auto it = find(candidates.begin(), candidates.end(), node);
            swap(*it, candidates.back());
            candidates.pop_back();


            continue;
        }
        result.emplace_back(node);
    }

    return result;
}

