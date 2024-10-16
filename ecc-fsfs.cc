#include "ecc-fsfs.h"

#include "graph.h"
#include "node.h"
#include "edge.h"
#include "clique.h"
#include "fast_set.h"
#include <gperftools/profiler.h>


ECC_FSFS::ECC_FSFS(string dataset_filepath) : ECC_FS(dataset_filepath) {
    dataset_filepath = dataset_filepath;
    G = new Graph(dataset_filepath);

    // add lookup fastsets
    candidates_fs = new Fast_set(G->_nodes.size());
    // lookup_set2 = new Fast_set(G->_nodes.size());
}

vector<Node *> ECC_FSFS::node_set_intersect(const vector<Node *>& vec_1, const vector<Node*>& vec_2) {
    if (vec_1.size() < vec_2.size()) {
        return node_set_intersect(vec_2, vec_1);
    }

    vector<Node*> intersection;
    intersection.reserve(vec_1.size());

    //Build lookup_sets
    lookup_set->clear();
    for (Node* node : vec_1) {
        lookup_set->add(node->index);
    }

    for (Node* node : vec_2) {
        if (lookup_set->get(node->index)) {
            intersection.push_back(node);
        }
    }
    return intersection;
}

void ECC_FSFS::trim_candidates(std::vector<Node*>& candidates, const std::vector<Node*>& trimming_vec) {
    // Clear the lookup set
    lookup_set->clear();

    // Add all indices from trimming_vec to the lookup set
    for (Node* node : trimming_vec) {
        lookup_set->add(node->index);
    }

    // Use the erase-remove idiom to remove candidates not in the lookup set
    candidates.erase(std::remove_if(candidates.begin(), candidates.end(), 
        [this](Node* node) {
            return !lookup_set->get(node->index); // Keep the node only if it's in the lookup set
        }), 
    candidates.end());
}


Clique* ECC_FSFS::find_clique_of(Edge* edge) {

    //R ← {u, v}  create the new clique with the edge
    Clique* clique = new Clique(edge, *G, num_edges_covered);
    Node* u = edge->_node1;
    Node* v = edge->_node2;

    // //save the clique (for deconstruction)
    // clique_cover.push_back(clique); 

    //P ← N(u)∩ N(v)
    vector<Node*> candidates = node_set_intersect(u->neighbors, v->neighbors);

    // z ← EXTRACT NODE(P)
    Node* new_member = extract_node(candidates, clique);

    // while there are more potential additions:
    while (new_member != nullptr) {
        //add new_node to clique (COVERS EDGES)
        clique->add_node(new_member, *G, num_edges_covered);

        // Trim candidates: P ← P ∩ N(z)
        // candidates = node_set_intersect(candidates, new_member->neighbors);
        // candidates = trim_set(candidates, new_member->neighbors);
        // trim_candidates(candidates, new_member->neighbor_set);

        trim_candidates(candidates, new_member->neighbors);

        // Extract next node
        // new_member = extract_node(candidates, clique);
        new_member = extract_node(candidates, clique);

    }
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
Node* ECC_FSFS::extract_node(vector<Node*>& potential_additions, Clique* clique) {

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