#ifndef NODE_H
#define NODE_H

class Edge; // avoid errors and circular definitions
class Graph; 
class Connection; 

#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
using namespace std;

class Node {
    public:
        Node(int id, int index_in_graph);
        Node();
        

        int id();
        int index; // the nodes index in the graph's node list
        vector<Edge*> get_edges(Graph &graph);
        vector<Node*> get_neighbors(Graph &graph);   // will be only for testing since adjacency list implemented

        vector<Node*> neighbors;                     // should be built with graph 
        vector<Edge*> edges; 

        vector<Connection*> connections;                // combining the adjacency lists into one

        //new datastructures for optimizing
        unordered_map<Node*, Edge*> connection_map;
        unordered_set<Node*> neighbor_set;


    private:
        int _id;

};
    
ostream& operator<<(ostream& os, Node& node);

bool operator==(Node& n1, Node& n2);

#endif