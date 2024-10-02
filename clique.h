#ifndef CLIQUE_H
#define CLIQUE_H

#include <vector>
#include <iostream>
using namespace std;

class Node;
class Edge;
class Graph;

class Clique {
public:

    Clique();
    Clique(vector<Node*> &nodes_to_add, Graph& G, size_t& edges_covered);
    Clique(Edge* edge, Graph& G, size_t& edges_covered);

    void add_node(Node* n, Graph& G, size_t& edges_covered);                    // add a node to the clique (adds all edges)

    bool is_complete(); // is the clique complete
    int size();


    vector<Node*> nodes; 
    vector<Edge*> edges;


private:

};
ostream& operator<<(ostream& os, Clique& clique);

#endif