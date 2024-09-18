#ifndef NODE_H
#define NODE_H

class Edge; // avoid errors and circular definitions
class Graph; // avoid errors and circular definitions

#include <iostream>
using namespace std;

class Node {
    public:
        Node(int id);
        Node();
        

        int id();
        vector<Edge*> get_edges(Graph &graph);
        vector<Node*> get_neighbors(Graph &graph);

        

    private:
        int _id;

};
    
ostream& operator<<(ostream& os, Node& node);

#endif