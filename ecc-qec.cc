#include "ecc-qec.h"

#include "graph.h"
#include "node.h"
#include "edge.h"
#include "clique.h"
#include "fast_set.h"
#include <gperftools/profiler.h>


ECC_QEC::ECC_QEC(string dataset_filepath) : ECC_FS(dataset_filepath) {
    // G = new Graph(dataset_filepath);
    node_clique_assignments = vector<int>(G->_nodes.size(), 0);
    name = "ECC_QEC";

}

ECC_QEC::ECC_QEC(Graph& G) : ECC_FS (G) {
    node_clique_assignments = vector<int>(G._nodes.size(), 0);
    name = "ECC_QEC";
}


/**
 * @brief Adds a node to the clique
 *    NEW: Instead of adding all edges and covering them, just adds node and updates node_clique_assignments
 *
 * @param clique 
 * @param node 
 */
void ECC_QEC::add_to_clique(Clique* clique, Node* node) {
    //Add node
    clique->nodes.push_back(node);
    node_clique_assignments[node->index] = clique->index;
}

/**
 * @brief After a clique has been made, covers all of the implied edges, adjusting num_edges_govered
 * 
 *
 * @param clique 
 */
void ECC_QEC::fill_clique(Clique* clique) {
    clique->edges.reserve(clique->edges.size() + clique->nodes.size()); // Reserve space if needed
    // Add edges
    for (int j = 0; j < clique->nodes.size(); j++){
        Node* node = clique->nodes[j];
        for (int i = j + 1; i < clique->nodes.size(); i++) {
            Edge* edge = G->get_edge(node, clique->nodes[i]);
            
            if (edge == nullptr) {
                cerr << "ERROR: Can't include " << *node << " in clique: Node " << *node << " is not connected to Node " << *clique->nodes[i] << endl;
            }

            if (!edge->is_covered()) {
                edge->cover();
                num_edges_covered += 1;
            }
            clique->edges.push_back(edge);
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
Clique* ECC_QEC::find_clique_of(Edge* edge) {
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