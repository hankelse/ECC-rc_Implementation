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
    Clique(vector<Node*> &nodes_to_add, const Graph& G, size_t& edges_covered);
    Clique(Edge* edge, const Graph& G, size_t& edges_covered);

    void add_node(Node* n, const Graph& G, size_t& edges_covered);                    // add a node to the clique (adds all edges)
    void add_node_cf(Node* n, const Graph& G, vector<size_t>& clique_counts);         //variation for ecc-cf

    bool is_complete(); // is the clique complete
    int size();


    int index;  // added for ECC-QEC, index of clique in list in G


    vector<Node*> nodes; 
    vector<Edge*> edges;


private:

};
ostream& operator<<(ostream& os, Clique& clique);

#endif