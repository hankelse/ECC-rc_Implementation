/*
New structure for talking about a node with respect to another. 
Used in node classes to refer to an edge connecting it to another node and the other node

*/

#include <iostream>
using namespace std;
#ifndef EV_PAIR_H
#define EV_PAIR_H

class Edge;
class Node;


class EV_pair {
public:
    Edge* edge;
    Node* neighbor;

    EV_pair();

    EV_pair(Edge* connecting_edge, Node* neighboring_node);

    bool is_covered();


};
ostream& operator<<(ostream& os, EV_pair& pair);




#endif