
#include "node.h"

Node::Node() {
    _id = 0; // default value
}

Node::Node(int id) {
    _id = id;
}



int Node::id() {
    return (_id);
}


ostream& operator<<(ostream& os, Node& node) {
    os << "[" << node.id() << "]";
    return os;
}