#include "connection.h"
#include "edge.h"
#include "node.h"


Connection::Connection() {
    edge = nullptr; 
    neighbor = nullptr;
}

Connection::Connection(Edge* connecting_edge, Node* neighboring_node) {
    edge = connecting_edge;
    neighbor = neighboring_node;
}

bool Connection::is_covered() {
    return edge->is_covered();
}

ostream& operator<<(ostream& os, Connection& pair) {
    Node* neighbor = pair.neighbor;
    Edge* edge = pair.edge;
    // os << "{->" << *neighbor << " by " << *edge << "}";
    os << "{self->" << *neighbor << "}";
    return os;
}