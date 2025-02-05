
#ifndef EDGE_H
#define EDGE_H
#include <iostream>
#include <vector>
using namespace std;

class Node;
class Connection;

class Edge {
    public:
        Edge();
        Edge(Node* node1, Node* node2);

        void cover();
        bool is_covered();
        Node* _node1;
        Node* _node2; 

        Connection* to_connection_of(Node& node);  //given a node, returns the connection of that node with the edge called on

        size_t index;
        bool _covered;

        // For fast data reductions:
        // All of the edges for whom this edge is between the common neighbors of its nodes
        vector<Edge*> counted_by;

    private:
        
};
    
ostream& operator<<(ostream& os, Edge& edge);

#endif