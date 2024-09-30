#include "clique.h"

#include "node.h"
#include "edge.h"
#include "graph.h"
#include "io.h"



Clique::Clique() {
}

/**
 * @brief Construct a new Clique with nodes in it
 * 
 * @param nodes_to_add 
 * @param G 
 */
Clique::Clique(vector<Node*> &nodes_to_add, Graph& G){ 
    for (int i = 0; i < nodes_to_add.size(); i++) {
        add_node(nodes_to_add[i], G);
    }
}

/**
 * @brief Add a node (and impplied edges) to a clique
 * 
 * @param n Node being added
 * @param G Graph
 */
void Clique::add_node(Node* n, Graph& G) {
    //Add edges
    for (int i = 0; i < nodes.size(); i++) {
        Edge* edge = G.get_edge(n, G._nodes[i]);
        if (edge == nullptr) {
            cerr << "ERROR: Can't include " << *n << " in clique: Node " << *n << " is not connected to Node " << *G._nodes[i] << endl;
        }
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
    os << "]>" << endl;
    return os;
}

