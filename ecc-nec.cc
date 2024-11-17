#include "ecc-nec.h"



#include "graph.h"
#include "node.h"
#include "edge.h"
#include "clique.h"
#include "fast_set.h"
#include "io.h"



ECC_NEC::ECC_NEC(string ds_filepath) : ECC_RED (ds_filepath) {
    name = "ECC_NEC";
    edge_intersection_counts = vector<size_t>(G->_edges.size(), 0);
    // node_removals = vector<bool>(G->_nodes.size(), 0);
    // edge_removals = vector<bool>(G->_edges.size(), 0);
    // apply_rule_two();

}

ECC_NEC::ECC_NEC(Graph& G) : ECC_RED (G) {
    name = "ECC_NEC";
    edge_intersection_counts = vector<size_t>(G._edges.size(), 0);
    // node_removals = vector<bool>(G._nodes.size(), 0);
    // edge_removals = vector<bool>(G._edges.size(), 0);
    // apply_rule_two();
}



vector<Clique*>* ECC_NEC::run() {

    //applies reductions
    // apply_rules_exhaustively();

    //sort edges
    init_edge_intersection_counts();
    get_edge_ordering();

    return ECC::run();
}

void ECC_NEC::init_edge_intersection_counts() {
    for (Edge* edge : G->_edges) {

        // skip if edge is covered
        if (edge->_covered) continue;
       
        // Get each node in the edge
        Node* n1 = edge->_node1;
        Node* n2 = edge->_node2;

        // Look at the intersection
        vector<Node*> common_neighbors = node_set_intersect(n1->neighbors, n2->neighbors);
        common_neighbors.push_back(n1);
        common_neighbors.push_back(n2);

        // calculate the number of edges between the common neighbors
        size_t edge_count = 0;
        for (int n1_index = 0; n1_index < common_neighbors.size(); n1_index++) {
            for (int n2_index = n1_index + 1; n2_index < common_neighbors.size(); n2_index++ ) {

                //is there an edge here?
                Edge* connecting_edge = G->get_edge(common_neighbors[n1_index], common_neighbors[n2_index]);
                if (connecting_edge != nullptr) {
                    edge_count += 1;
                }
            }
        }

        //Put the value in the list
        edge_intersection_counts[edge->index] = edge_count;
    }

}

/**
 * @brief Makes edge_ordering a list of edges sorted by the values in edge-intersection-counts
 * 
 */
void ECC_NEC::get_edge_ordering() {

    // Copy all edges to edge_ordering
    edge_ordering = G->_edges; 
    const Graph* G = this->G;
    vector<size_t> edge_counts = edge_intersection_counts;

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

/**
 * @brief selects an uncovered edge in G and keeps track of where it left off
 * NEW: Selects in order of the edge ordering
 * 
 * @param previous_index 
 * @return Edge* 
 */
Edge* ECC_NEC::select_uncovered_edge(int& previous_index) {
    for (int i = previous_index; i < edge_ordering.size(); i++) {
        if (!edge_ordering[i]->is_covered()) {
            previous_index = i;
            return edge_ordering[i];
        }
    }
    cerr << "Uh Oh!  Got to end of select_uncovered_edge!" << endl;
    return nullptr;
}
    
