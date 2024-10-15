#include "ecc.h"

#include "graph.h"
#include "node.h"
#include "edge.h"
#include "clique.h"



// ==== PUBLIC METHODS ==== //

/**
 * @brief Runs the algorith using the framework for Conte et al.
 * 
 *
 * @return 
 */
vector<Clique*>* ECC::run(string prof_out_filepath) {
    if (prof_out_filepath != "None") {
        ProfilerStart("profile_output.prof");
    }


    num_edges_covered = 0;
    int last_uncovered_edge_index = 0;

    size_t round = 0;
    while (num_edges_covered < G->_edges.size()) {
        round ++;

        // u, v ← SELECT UNCOVERED EDGE()
        Edge* uncovered_edge = select_uncovered_edge(last_uncovered_edge_index);


        //R ← FIND CLIQUE OF(u, v)
        //Increases num_edges_covered as it runs
        Clique* found_clique = find_clique_of(uncovered_edge);
        // C ← C∪R
        clique_cover.push_back(found_clique);

    }
    ProfilerStop();
    return &clique_cover;
}


/**
 * @brief Checks if solution is correct
 * 
 *
 * @return 
 */
bool ECC::check_solution () {
    for (Edge* edge : G->_edges) {
        if (!edge->is_covered()) {
            return false;
        }
    }
    return true;
}

/**
 * @brief Constructor of ECC Object: Builds graph from file
 * 
 *
 * @param dataset_filepath filepath of dataset
 */
ECC::ECC(string dataset_filepath) {
    _dataset_filepath = dataset_filepath;
    G = new Graph(dataset_filepath);
}


// ==== FRAMEWORK METHODS ==== //

/**
 * @brief selects an uncovered edge in G starting where it left off.
 * 
 * @param previous_index 
 * @return Edge* 
 */
Edge* ECC::select_uncovered_edge(int& previous_index) {
    for (int i = previous_index; i < G->_edges.size(); i++) {
        if (!G->_edges[i]->is_covered()) {
            previous_index = i;
            return G->_edges[previous_index];
        }
    }
    cerr << "Uh Oh!  Got to end of select_uncovered_edge!" << endl;
    return nullptr;
}

/**
 * @brief Creates a Clique object representing the clique with the edge
 * 
 * @param edge_uv 
 * @return vector<Edge*> 
 */
Clique* ECC::find_clique_of(Edge* edge) {

    //R ← {u, v}  create the new clique with the edge
    Clique* clique = new Clique(edge, *G, num_edges_covered);
    Node* u = edge->_node1;
    Node* v = edge->_node2;

    // //save the clique (for deconstruction)
    // clique_cover.push_back(clique); 

    //P ← N(u)∩ N(v)
    unordered_set<Node*> candidates = node_set_intersect(u->neighbor_set, v->neighbor_set);

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
        candidates = node_set_intersect(candidates, new_member->neighbor_set);

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
Node* ECC::extract_node(unordered_set<Node*>& potential_additions, Clique* clique) {

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


// ===== HELPER FUNCTIONS ===== //

/**
 * @brief Finds the intersection between two sets of node*s
 * 
 *
 * @param set_1 
 * @param set_2 
 * @return 
 */
unordered_set<Node *> ECC::node_set_intersect(unordered_set<Node *>& set_1, unordered_set<Node *>& set_2) {

    //Recursively run with smaller first
    if (set_1.size() > set_2.size()) {
        return node_set_intersect(set_2, set_1);
    }

    unordered_set<Node*> result;
    
    // Iterate over the smaller set and check for each node in larger
    for (Node* node : set_1) {
        if (set_2.count(node) > 0) {
            result.insert(node);
        }
    }
    return result;
}
