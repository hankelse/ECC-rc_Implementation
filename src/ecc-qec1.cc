#include "ecc-qec1.h"

#include "graph.h"
#include "node.h"
#include "edge.h"
#include "clique.h"
#include "fast_set.h"
// #include <gperftools/profiler.h>



ECC_QEC1::ECC_QEC1(string dataset_filepath) : ECC_FS(dataset_filepath) {
    // G = new Graph(dataset_filepath);
    node_clique_assignments = vector<int>(G->_nodes.size(), 0);
    name = "ECC_QEC1";

}

ECC_QEC1::ECC_QEC1(Graph& G) : ECC_FS (G) {
    node_clique_assignments = vector<int>(G._nodes.size(), 0);
    name = "ECC_QEC1";
}


/**
 * @brief Adds a node to the clique
 *    NEW: Instead of adding all edges and covering them, just adds node and updates node_clique_assignments
 *
 * @param clique 
 * @param node 
 */
void ECC_QEC1::add_to_clique(Clique* clique, Node* node) {
    //Add node
    clique->nodes.push_back(node);
    node_clique_assignments[node->index] = clique->index;
}

inline bool covered(Edge* edge) {return edge->_covered;}
inline void cover(Edge* edge) {edge->_covered = true;}


/**
 * @brief After a clique has been made, covers all of the implied edges, adjusting num_edges_govered
 * 
 *
 * @param clique 
 */
void ECC_QEC1::fill_clique(Clique* clique) {
    // clique->edges.reserve(clique->edges.size() + clique->nodes.size()); // Reserve space if needed
    // Add edges
    for (int j = 0; j < clique->nodes.size(); j++){
        Node* node = clique->nodes[j];
        for (int i = j + 1; i < clique->nodes.size(); i++) {
            Edge* edge = G->get_edge(node, clique->nodes[i]);
            
            if (edge == nullptr) {
                cerr << "ERROR: Can't include " << *node << " in clique: Node " << *node << " is not connected to Node " << *clique->nodes[i] << endl;
            }

            if (!covered(edge)) {
                cover(edge);
                num_edges_covered += 1;
            }
            // clique->edges.push_back(edge);
        }
    }

}


/**
 * @brief Finds the clique of a given edge
 * NEW: Saves the index of the Clique in clique_cover
 *
 * @param edge 
 * @return 
 */
Clique* ECC_QEC1::find_clique_of(Edge* edge) {
    //R ← {u, v}  create the new clique with the edge
    Clique* clique = new Clique();
    clique->index = clique_cover.size();  // New to QEC: adds the index of the clique 

    Node* u = edge->_node1;
    Node* v = edge->_node2;

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

// /**
//  * @brief Extract a node from potential_additions based whichever maximizes |N_u(z) ∩ R| > 0; 
//  * otherwise, returns z = null.
//  * NEW: Checks if an edge is covered using the node_clique_assignment vector
//  *
//  * @param potential_additions 
//  * @param clique 
//  * @return 
//  */
// Node* ECC_QEC1::extract_node(vector<Node*>& potential_additions, Clique* clique) {

//     //If no potential_additions
//     if (potential_additions.empty()) {
//         return nullptr;
//     }

//     // Create a vector to track the number of uncovered connections between each potential node and the clique nodes
//     unordered_map<Node*, int> uncovered_counts;

//     // Iterate over the clique's nodes and check for uncovered edges
//     for (Node* clique_node : clique->nodes) {
//         for (Node* potential : potential_additions) {
//             // Use are_connected() to check if the node is connected to the clique node and if the edge is uncovered
//             Edge* connecting_edge = G->are_connected(potential, clique_node);
//             if (connecting_edge != nullptr && !connecting_edge->is_covered()) {
//                 uncovered_counts[potential] += 1;
//             }
//         }
//     }

//     // Find the node with the maximum uncovered connections
//     Node* best_node = nullptr;
//     int best_score = 0;
    
//     for (const auto& [node, score] : uncovered_counts) {
//         if (score > best_score) {
//             best_node = node;
//             best_score = score;
//         }
//     }

//     return best_node;
// }


