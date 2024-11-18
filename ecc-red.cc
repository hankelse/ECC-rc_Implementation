#include "ecc-red.h"



#include "graph.h"
#include "node.h"
#include "edge.h"
#include "clique.h"
#include "fast_set.h"
#include "io.h"



ECC_RED::ECC_RED(string ds_filepath) : ECC_QEC1 (ds_filepath) {
    name = "ECC_RED";
    node_removals = vector<bool>(G->_nodes.size(), 0);
    edge_removals = vector<bool>(G->_edges.size(), 0);
    // apply_rule_two();

}

ECC_RED::ECC_RED(Graph& G) : ECC_QEC1 (G) {
    name = "ECC_RED";
    node_removals = vector<bool>(G._nodes.size(), 0);
    edge_removals = vector<bool>(G._edges.size(), 0);
    // apply_rule_two();
}


vector<Clique*>* ECC_RED::run(){
    size_t og_num_nodes = G->_nodes.size();
    size_t og_num_edges = G->_edges.size();

    // apply reductions exhaustively
    apply_rules_exhaustively();


    cout << "\t\tPreformed Data Reductions:" << endl;
    cout << "\t\t\t Reduced nodes: " << og_num_nodes << " -> " << G->_nodes.size() - nodes_removed << " (by " << nodes_removed << ")." << endl;
    cout << "\t\t\t Covered: " << num_edges_covered << " edges." << endl;
    cout << "\t\t\t Finding " << clique_cover.size() << " cliques." << endl;


    ECC::run();
    return &clique_cover;
}



size_t ECC_RED::apply_rule_two() {
    size_t cliques_found = 0;

    Clique* found_clique;
    for (Node* node : G->_nodes) {
        if (node_removals[node->index]) continue; // skip node if already removed

        for (const auto & [neighbor, connecting_edge] : node->connection_map) {
            if (node_removals[neighbor->index]) continue; // skip neighbor if already removed
            if (connecting_edge->is_covered()) continue; //skip neighbor if connecting edge already covered.

            //compute common neighbors
            vector<Node*> common_neighbors = node_set_intersect(node->neighbors, neighbor->neighbors);

            common_neighbors.push_back(node);
            common_neighbors.push_back(neighbor);

            //check if common_neighbors is a clique
            if (is_clique (common_neighbors)) {

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


/**
 * @brief Finds the intersection of two vectors of node*s. Checks if nodes are still in G
 * 
 *
 * @param vec_1 the first vec of nodes
 * @param vec_2 the second vec of nodes
 * @return 
 */
vector<Node *> ECC_RED::node_set_intersect(const vector<Node *>& vec_1, const vector<Node*>& vec_2) {
    //Iterate through the smaller vector
    if (vec_1.size() < vec_2.size()) {
        return node_set_intersect(vec_2, vec_1);
    }

    //Initialize the intersection to the size of the smaller vector
    vector<Node*> intersection;
    intersection.reserve(vec_2.size());

    //Update lookup set with vec_1's nodes
    lookup_set->clear();
    for (Node* node : vec_1) {
        // If the node is not removed
        if (not is_removed(node)) {
            lookup_set->add(node->index);
        }
    }

    //Iterate through vec_2 checking in the lookup set for mutual neighbors
    for (Node* node : vec_2) {
        if (lookup_set->get(node->index)) {
            intersection.push_back(node);
        }
    }
    return intersection;
}


void ECC_RED::trim_candidates(std::vector<Node*>& candidates, const std::vector<Node*>& trimming_vec) {
    // Clear the lookup set
    lookup_set->clear();

    // Add all indices from trimming_vec to the lookup set
    for (Node* node : trimming_vec) {
        if (!is_removed(node)) {
            lookup_set->add(node->index);
        }
    }
    // Use the erase-remove idiom to remove candidates not in the lookup set
    candidates.erase(std::remove_if(candidates.begin(), candidates.end(), 
        [this](Node* node) {
            return !lookup_set->get(node->index); // Keep the node only if it's in the lookup set
        }), 
    candidates.end());
}



Clique* ECC_RED::find_clique_of(Edge* edge) {
    //R ← {u, v}  create the new clique with the edge
    Clique* clique = new Clique();
    clique->index = clique_cover.size();  // New to QEC: adds the index of the clique 

    Node* u = edge->_node1;
    Node* v = edge->_node2;

    if (is_removed(u) || is_removed(v)) {
        throw invalid_argument("Passed edge with removed nodes");
    }

    add_to_clique(clique, u);
    add_to_clique(clique, v);

    //P ← N(u)∩ N(v)
    vector<Node*> candidates = node_set_intersect(u->neighbors, v->neighbors);

    // z ← EXTRACT NODE(P)
    Node* new_member = extract_node(candidates, clique);

    // while there are more potential additions:
    while (new_member != nullptr) {
        //add new_node to clique (COVERS EDGES)
        add_to_clique(clique, new_member);

        // Trim candidates: P ← P ∩ N(z)
        // candidates = node_set_intersect(candidates, new_member->neighbors);
        // candidates = trim_set(candidates, new_member->neighbors);
        // trim_candidates(candidates, new_member->neighbor_set);

        trim_candidates(candidates, new_member->neighbors);

        // Extract next node
        // new_member = extract_node(candidates, clique);
        new_member = extract_node(candidates, clique);

    }
    fill_clique(clique);
    return clique;
}



/**
 * @brief Extract a node from potential_additions based whichever maximizes |N_u(z) ∩ R| > 0; 
 * otherwise, returns z = null.
 *
 * @param potential_additions 
 * @param clique 
 * @return 
 */
Node* ECC_RED::extract_node(vector<Node*>& potential_additions, Clique* clique) {
    //If no potential_additions
    if (potential_additions.empty()) {
        return nullptr;
    }

    // Create a vector to track the number of uncovered connections between each potential node and the clique nodes
    unordered_map<Node*, int> uncovered_counts;

    // Iterate over the clique's nodes and check for uncovered edges
    for (Node* clique_node : clique->nodes) {
        for (Node* potential : potential_additions) {
            // Use are_connected() to check if the node is connected to the clique node and if the edge is uncovered
            Edge* connecting_edge = G->are_connected(potential, clique_node);
            if (connecting_edge != nullptr && !connecting_edge->is_covered()) {
                uncovered_counts[potential] += 1;
            }
        }
    }

    // Find the node with the maximum uncovered connections
    Node* best_node = nullptr;
    int best_score = 0;
    
    for (const auto& [node, score] : uncovered_counts) {
        if (score > best_score) {
            best_node = node;
            best_score = score;
        }
    }

    return best_node;
}

bool ECC_RED::is_removed(Node* node) {
    return node_removals[node->index];
}


size_t ECC_RED::apply_rule_one() {
    size_t starting_num_removed = nodes_removed;
    
    for (Node* node : G->_nodes) {
        if (is_removed(node)) continue; // Skip node if removed

        //check if all incident edges have been covered
        vector<Edge*> incident_edges = node->edges;
        bool all_covered = true;
        for (Edge* edge : incident_edges) {
            if (!edge->_covered) {
                all_covered = false;
                break;
            }
        }

        //if all incident edges covered, remove node from G
        if (all_covered) {
            size_t index = node->index;
            node_removals[index] = true;
            nodes_removed ++;
        }
    }

    return nodes_removed - starting_num_removed;
}


void ECC_RED::apply_rules_exhaustively() {
    size_t change = 1;
    while (change > 0) {
        change = 0;
        change += apply_rule_two();
        change += apply_rule_one();
    }
    cout << num_edges_covered << endl;
    cout << nodes_removed << endl;
}



// bool ECC_RED::is_clique(vector<Node*>& nodes) {
//     cout << "\t\t CHECKING IF " << nodes << " is a CLIQUE!" << endl;
//     for (int n1_index = 0; n1_index < nodes.size(); n1_index++) {
//         Node* n1 = G->_nodes[n1_index];
//         for (int n2_index = n1_index+1; n2_index < nodes.size(); n2_index++) {
//             Node* n2 = G->_nodes[n2_index];
//             Edge* connecting_edge = G->get_edge(n1, n2);
//             if (connecting_edge == nullptr || is_removed(n1) || is_removed(n2)) {
//                 cout << "\tNOPE! " << *n1 << " " << *n2 << endl;
//                 return false;
//             }
//         }
//     }
//     return true;
// }

bool ECC_RED::is_clique(vector<Node*>& nodes) {
    // cout << "\t\t CHECKING IF " << nodes << " is a CLIQUE!" << endl;
    for (int n1_index = 0; n1_index < nodes.size(); n1_index++) {
        Node* n1 = nodes[n1_index];
        for (int n2_index = n1_index+1; n2_index < nodes.size(); n2_index++) {
            Node* n2 = nodes[n2_index];
            Edge* connecting_edge = G->get_edge(n1, n2);
            if (connecting_edge == nullptr || is_removed(n1) || is_removed(n2)) {
                // cout << "\tNOPE! " << *n1 << " " << *n2 << endl;
                return false;
            }
        }
    }
    return true;
}