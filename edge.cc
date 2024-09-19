
#include "edge.h"
#include "node.h"

Edge::Edge() {
    _covered = false;
}

Edge::Edge(Node* node1, Node* node2) {
    _node1 = node1;
    _node2 = node2;
    
    _covered = false;
}

bool Edge::is_covered() {
    return _covered;
}

void Edge::cover() {
    _covered = true;
}

ostream& operator<<(ostream& os, Edge& edge) {
    Node* node1 = edge._node1;
    Node* node2 = edge._node2;
    os << "{" << node1->id() << "<->" << node2->id() << "}";
    return os;
}


