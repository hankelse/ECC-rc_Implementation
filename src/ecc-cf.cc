#include "ecc-cf.h"

#include "graph.h"
#include "node.h"
#include "edge.h"
#include "clique.h"
#include "fast_set.h"
#include "io.h"
// #include <gperftools/profiler.h>


/**
 * @brief Runs the algorith using the framework for Conte et al.
 *  NEW: uses clique_counts instead of num_edges_covered
 *
 * @return 
 */
vector<Clique*>* ECC_CF::run() {

    // num_edges_covered = 0;
    int last_uncovered_edge_index = 0;

    size_t round = 0;

    bool edges_still_uncovered = true;
    while (edges_still_uncovered) {
        round ++;

        // u, v ← SELECT UNCOVERED EDGE()
        Edge* uncovered_edge = select_uncovered_edge(last_uncovered_edge_index);
        if (uncovered_edge == nullptr) {
            edges_still_uncovered = false;
            continue;
        }


        //R ← FIND CLIQUE OF(u, v)
        Clique* found_clique = find_clique_of(uncovered_edge);


        // C ← C∪R
        clique_cover.push_back(found_clique);

    }
    return &clique_cover;
}


ECC_CF::ECC_CF(string dataset_filepath) : ECC_FS(dataset_filepath) {

    // NEW: Frequency vector for number of cliques each edge is in
    clique_counts = vector<size_t>(G->_edges.size()+1, 0);
    name = "ECC_CF";

}

ECC_CF::ECC_CF (Graph& Graph) : ECC_FS(Graph) {
    clique_counts = vector<size_t>(G->_edges.size()+1, 0);
    name = "ECC_CF";
}

/**
 * @brief Selects an uncovered edge
 * NOW: Uses the clique_counts vector to check if an edge has been covered
 * 
 *
 * @param previous_index 
 * @return 
 */
Edge* ECC_CF::select_uncovered_edge(int& previous_index) {
    for (int i = previous_index; i < G->_edges.size(); i++) {
        if (clique_counts[i] == 0) {
            previous_index = i;
            return G->_edges[previous_index];
        }
    }
    return nullptr;  //intentionally returns nullptr
}

/**
 * @brief Finds the clique of a given edge!
 * NEW:  Implements new add_node_cf which increments the clique_counts
 *       Uses default constructor so add_node_cf can be used
 * 
 * @param edge 
 * @return 
 */
Clique* ECC_CF::find_clique_of(Edge* edge) {

    //R ← {u, v}  create the new clique with the edge
    Clique* clique = new Clique();
    Node* u = edge->_node1;
    Node* v = edge->_node2;
    clique->add_node_cf(u, *G, clique_counts);
    clique->add_node_cf(v, *G, clique_counts);

    //P ← N(u)∩ N(v)
    vector<Node*> candidates = node_set_intersect(u->neighbors, v->neighbors);

    // z ← EXTRACT NODE(P)
    Node* new_member = extract_node(candidates, clique);

    // while there are more potential additions:
    while (new_member != nullptr) {
        //add new_node to clique (COVERS EDGES)
        clique->add_node_cf(new_member, *G, clique_counts);

        // Trim candidates: P ← P ∩ N(z)

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
 *  NEW: using clique_counts instead of edge->covered
 *
 * @param potential_additions 
 * @param clique 
 * @return 
 */
Node* ECC_CF::extract_node(vector<Node*>& potential_additions, Clique* clique) {

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
            if (connecting_edge != nullptr && !clique_counts[connecting_edge->index]) {
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



bool ECC_CF::check_solution() {
    for (int i = 0; i < G->_edges.size(); i++) {
        if (clique_counts[i] == 0) {
            return false;
        }
    }
    return true;
}
void ECC_CF::check_solution_debug() {
    vector<Edge*> uncovered_edges;
    for (Edge* edge : G->_edges) {
        if (clique_counts[edge->index] == 0) {
            uncovered_edges.push_back(edge);
        }
    }
    cout << "DEBUGGING INCORRECT SOLUTION" << endl;
    cout << "\t" << uncovered_edges.size() << " edges left uncovered." << endl;
    for (Edge* edge : uncovered_edges) {
        cout << "\t\t Graph._edges[" << edge->index << "] = " << *edge << endl;
    }
    cout << "\t" << uncovered_edges.size() << " out of " << G->_edges.size() << " left uncovered." << endl;
}


// vector<Node *> ECC_CF::node_set_intersect(const vector<Node *>& vec_1, const vector<Node*>& vec_2) {
//     if (vec_1.size() < vec_2.size()) {
//         return node_set_intersect(vec_2, vec_1);
//     }

//     vector<Node*> intersection;
//     intersection.reserve(vec_1.size());

//     //Build lookup_sets
//     lookup_set->clear();
//     for (Node* node : vec_1) {
//         lookup_set->add(node->index);
//     }

//     for (Node* node : vec_2) {
//         if (lookup_set->get(node->index)) {
//             intersection.push_back(node);
//         }
//     }
//     return intersection;
// }

// void ECC_CF::trim_candidates(std::vector<Node*>& candidates, const std::vector<Node*>& trimming_vec) {
//     // Clear the lookup set
//     lookup_set->clear();

//     // Add all indices from trimming_vec to the lookup set
//     for (Node* node : trimming_vec) {
//         lookup_set->add(node->index);
//     }

//     // Use the erase-remove idiom to remove candidates not in the lookup set
//     candidates.erase(std::remove_if(candidates.begin(), candidates.end(), 
//         [this](Node* node) {
//             return !lookup_set->get(node->index); // Keep the node only if it's in the lookup set
//         }), 
//     candidates.end());
// }

