/*
New structure for talking about a node with respect to another. 
Used in node classes to refer to an edge connecting it to another node and the other node

*/

#ifndef EV_PAIR_H
#define EV_PAIR_H

#include "edge.h";
#include "node.h";


class EV_pair {
public:

    Edge* edge = nullptr; 
    Node* neighbor = nullptr;

};




#endif