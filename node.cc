
#include "node.h"

Node::Node() {
    _value = 0; // default value
}

Node::Node(int value) {
    _value = value;
}

int Node::value() {
    return (_value);
}


