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

        vector<Edge*> _edges;
        vector<Node*> _nodes;
        int _num_nodes;
        int _num_edges;
        
    private:
        void construct(string filename);
        void get_sizes(ifstream& file);
        void fill_graph(ifstream& file);
};
    


#endif