#ifndef CHECKS_H
#define CHECKS_H

#include <sstream>
using namespace std;

class Graph;

/* Was building the graph done correctly? */

bool all_nodes_imported(Graph& G, ostringstream& warnings); // Do we have as many nodes as we are supposed to?
bool all_edges_imported(Graph& G, ostringstream& warnings); // Do we have as many edges as we are supposed to?

/* Where the adjacency lists for the nodes built correclty? */

bool adj_lists_correct(Graph& G, ostringstream& warnings);


/* helper functions */
template <typename S>
bool vectors_contents_same(vector<S>&, vector<S>&); 

#endif