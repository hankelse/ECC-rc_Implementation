
#ifndef EDGE_H
#define EDGE_H
#include <iostream>
#include <vector>
using namespace std;
#include "node.h"

class Edge {
    public:
        Edge();
        Edge(Node* node1, Node* node2);

        int cover();
        bool is_covered();

        Node* _node1;
        Node* _node2; 
    
    private:
        bool _covered;


    
};
    
ostream& operator<<(ostream& os, Edge& edge);

#endif