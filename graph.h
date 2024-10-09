#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <string>
#include <fstream>
#include <unordered_set>

class Connection;
class Node;
class Edge;
class Clique;

using namespace std;


class Graph {
    public:
        Graph();
        Graph(string filename);
        ~Graph();

        vector<Clique*> _cliques;
        vector<Edge*> _edges;
        vector<Node*> _nodes;
        int _num_nodes;
        int _num_edges;


        //ECC-rc methods
        Edge* select_uncovered_edge(int& previous_index);
        Clique* find_clique_of(Edge* edge, size_t& edges_covered);
        Clique* demo_find_clique_of(Edge* edge, size_t& edges_covered);
        Node* extract_node(vector<Node*>& candidate_set, Clique* clique);
        Node* extract_node(unordered_set<Node*>& candidate_set, Clique* clique);

        Edge* are_connected(Node* node1, Node* node2);
        Edge* get_edge(Node* node1, Node* node2);  // synonymous to are_connected


        //helper methods that probably shouldn't be used
        vector<Node*> edges_to_nodes(vector<Edge*> edges);
        vector<Edge*> nodes_to_edges(vector<Node*> nodes);

        vector<Node*> node_set_intersect(vector<Node*>&, vector<Node*>&);
        unordered_set<Node *> trim_set(unordered_set<Node *>& set_1, vector<Node*>& vec_2);


    private:
        void construct(string filename);       //fills graph using get_data_sizes and fill_graph
        void get_data_sizes(ifstream& file);   //gets sizes from file of _nodes and _edges
        void fill_graph(ifstream& file);       //puts nodes and edges into graph
};




#endif