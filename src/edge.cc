
#include "edge.h"
#include "node.h"
#include "connection.h"

Edge::Edge() {
    _covered = false;
    counted_by = {};
}

Edge::Edge(Node* node1, Node* node2) {
    _node1 = node1;
    _node2 = node2;
    
    _covered = false;
    counted_by = {};
}

bool Edge::is_covered() {
    return _covered;
}

void Edge::cover() {
    _covered = true;
}


/**
 * @brief Given a node in the edge, returns the connection from the nodes connections containing the edge
 * 
 * @param node 
 * @return Connection* 
 */
//Could be implemented faster if nodes and edges are connected
Connection* Edge::to_connection_of(Node& node) {
    if (node.id() == _node1->id()) {
        //if node is node1
        for (int i = 0; i < node.connections.size(); i++) {
            Connection* connection = node.connections[i];
            //if the connection's edge has node2 as its node2
            if (connection->edge->_node2->id() == _node2->id()) {
                return node.connections[i];
            }
        }

    } else if (node.id() == _node2->id()) {
        //if node is node2
        for (int i = 0; i < node.connections.size(); i++) {
            Connection* connection = node.connections[i];
            //if the connection's edge has node1 as its node1
            if (connection->edge->_node1->id() == _node1->id()) {
                return node.connections[i];
            }
        }
    } 
    cerr << "Edge::to_connection_of was passed a node that wasn't in it" << endl;
    return nullptr;
}




ostream& operator<<(ostream& os, Edge& edge) {
    Node* node1 = edge._node1;
    Node* node2 = edge._node2;
    os << "{" << node1->id() << "<->" << node2->id() << "}";
    return os;
}



