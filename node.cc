
#include "node.h"
#include "edge.h"
#include "graph.h"

Node::Node() {
    _id = 0; // default value
}

Node::Node(int id) {
    _id = id;
}


/**
 * @brief Returns a list of edges that a node is a part of
 * 
 * @param node The node to find the edges of
 * @return vector<Edge*> 
 */
vector<Edge*> Node::get_edges(Graph &graph) {
    vector<Edge*> result;
    Edge* current_edge;

    for (int i = 0; i < graph._num_edges; i++) {
        current_edge = graph._edges[i];

        // if node is in the edge bring looked at, save the edge.
        if (current_edge->_node1->id() == _id) {
            result.push_back(current_edge);
        }
        else if (current_edge->_node2->id() == _id) {
            result.push_back(current_edge);
        }
    }
    return result;
}

/**
 * @brief Returns a list of neighbors of the node.
 * 
 * @param node The node to find the edges of
 * @return vector<Edge*> 
 */
vector<Node*> Node::get_neighbors(Graph &graph) {
    vector<Node*> neighbors; 
    Edge* current_edge;
    for (int i = 0; i < graph._num_edges; i++) {
        current_edge = graph._edges[i];
        if (current_edge->_node1->id() == _id) {
            neighbors.push_back(current_edge->_node2);
        }
        else if (current_edge->_node2->id() == _id) {
            neighbors.push_back(current_edge->_node1);
        }
    }
    return neighbors;
}



int Node::id() {
    return (_id);
}


ostream& operator<<(ostream& os, Node& node) {
    os << "[" << node.id() << "]";
    return os;
}