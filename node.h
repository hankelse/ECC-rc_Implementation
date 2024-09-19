#ifndef NODE_H
#define NODE_H

class Edge; // avoid errors and circular definitions
class Graph; 
class EV_pair; 

#include <iostream>
#include <vector>
using namespace std;

class Node {
    public:
        Node(int id);
        Node();
        

        int id();
        vector<Edge*> get_edges(Graph &graph);
        vector<Node*> get_neighbors(Graph &graph);   // will be only for testing since adjacency list implemented

        vector<Node*> neighbors;                     // should be built with graph 
        vector<Edge*> edges; 

        vector<EV_pair*> connections;                // combinding the adjacency lists into one


    private:
        int _id;

};
    
ostream& operator<<(ostream& os, Node& node);

#endif