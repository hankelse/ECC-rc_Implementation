#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <string>
#include <fstream>

class Connection;
class Node;
class Edge;

using namespace std;


class Graph {
    public:
        Graph();
        Graph(string filename);
        ~Graph();

        vector<Edge*> _edges;
        vector<Node*> _nodes;
        int _num_nodes;
        int _num_edges;

        //ECC-rc methods
        Edge* select_uncovered_edge(int& previous_index);
        vector<Edge*> find_clique_of(Edge* uncovered_edge);
        Node* extract_node(vector<Node*> node_set, vector<Node*>& result);

        Edge* are_connected(Node* node1, Node* node2);

        //helper methods that probably shouldn't be used
        vector<Node*> edges_to_nodes(vector<Edge*> edges);
        vector<Edge*> nodes_to_edges(vector<Node*> nodes);


    private:
        void construct(string filename);       //fills graph using get_data_sizes and fill_graph
        void get_data_sizes(ifstream& file);   //gets sizes from file of _nodes and _edges
        void fill_graph(ifstream& file);       //puts nodes and edges into graph
};

vector<Node*> node_set_intersect(vector<Node*> u_neighbors, vector<Node*> v_neighbors);


#endif