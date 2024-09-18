#ifndef CHECKS_H
#define CHECKS_H

#include "graph.h"
#include <sstream>
using namespace std;

/* Was building the graph done correctly? */

bool all_nodes_imported(Graph& G, ostringstream& warnings); // Do we have as many nodes as we are supposed to?
bool all_edges_imported(Graph& G, ostringstream& warnings); // Do we have as many edges as we are supposed to?

/* Where the adjacency lists for the nodes built correclty? */

bool adj_lists_correct(Graph& G, ostringstream& warnings);

#endif