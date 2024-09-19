#include "ev_pair.h"
#include "edge.h"
#include "node.h"


EV_pair::EV_pair() {
    edge = nullptr; 
    neighbor = nullptr;
}

EV_pair::EV_pair(Edge* connecting_edge, Node* neighboring_node) {
    edge = connecting_edge;
    neighbor = neighboring_node;
}

bool EV_pair::is_covered() {
    return edge->is_covered();
}

ostream& operator<<(ostream& os, EV_pair& pair) {
    Node* neighbor = pair.neighbor;
    Edge* edge = pair.edge;
    // os << "{->" << *neighbor << " by " << *edge << "}";
    os << "{self->" << *neighbor << "}";
    return os;
}