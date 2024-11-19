/*
New structure for talking about a node with respect to another. 
Used in node classes to refer to an edge connecting it to another node and the other node

*/

#include <iostream>
using namespace std;
#ifndef CONNECTION_H
#define CONNECTION_H

class Edge;
class Node;


class Connection {
public:
    Edge* edge;
    Node* neighbor;

    Connection();
    Connection(Edge* connecting_edge, Node* neighboring_node);

    bool is_covered();


};
ostream& operator<<(ostream& os, Connection& pair);




#endif