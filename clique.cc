#include "clique.h"

#include "node.h"
#include "edge.h"
#include "graph.h"
#include "io.h"



// Clique::Clique() {
// }

// /**
//  * @brief Construct a new Clique with nodes in it
//  * 
//  * @param nodes_to_add 
//  * @param G 
//  */
// Clique::Clique(vector<Node*> &nodes_to_add, Graph& G, size_t& edges_covered){ 
//     for (int i = 0; i < nodes_to_add.size(); i++) {
//         add_node(nodes_to_add[i], G, edges_covered);
//     }
// }


Clique::Clique(Edge* edge, Graph& G, size_t &edges_covered) {
    nodes = {};
    add_node(edge->_node1, G, edges_covered);
    add_node(edge->_node2, G, edges_covered);
}

/**
 * @brief Add a node (and impplied edges) to a clique (COVERS EDGES)
 * 
 * @param n Node being added
 * @param G Graph
 */
void Clique::add_node(Node* n, Graph& G, size_t& edges_covered) {
    //reserve space
    edges.reserve(edges.size() + nodes.size()); // Reserve space if needed
    //Add edges
    for (int i = 0; i < nodes.size(); i++) {
        Edge* edge = G.get_edge(n, nodes[i]);
        
        if (edge == nullptr) {
            cerr << "ERROR: Can't include " << *n << " in clique: Node " << *n << " is not connected to Node " << *nodes[i] << endl;
        }

        if (!edge->is_covered()) {
            edge->cover();
            edges_covered += 1;
            
        }
        // edge->cover();
        edges.push_back(edge);
    }

    //Add node
    nodes.push_back(n);
}

/**
 * @brief Is the clique complete; Is every node connected to every other?
 * 
 * @return true 
 * @return false 
 */
bool Clique::is_complete() {
    cerr << "This isn't implemented!" << endl;
    return false;
}


int Clique::size() {
    return edges.size();
}

/**
 * @brief Overloading operator for outputting Cliques
 * 
 * @param os 
 * @param clique 
 * @return ostream& 
 */
ostream& operator<<(ostream& os, Clique& clique) {
    os << "<[" << clique.nodes[0]->id();
    for (int i = 1; i < clique.nodes.size(); i++) {
        os << "x" << clique.nodes[i]->id();
    }
    os << "]>";
    return os;
}

