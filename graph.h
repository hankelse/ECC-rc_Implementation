#ifndef GRAPH_H
#define GRAPH_H

#include "node.h"
#include "edge.h"
#include <vector>
#include <string>
#include <fstream>

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
        Edge* select_uncovered_edge();

    private:
        void construct(string filename);       //fills graph using get_data_sizes and fill_graph
        void get_data_sizes(ifstream& file);   //gets sizes from file of _nodes and _edges
        void fill_graph(ifstream& file);       //puts nodes and edges into graph
};
    


#endif