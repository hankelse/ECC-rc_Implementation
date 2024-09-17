#include <iostream>
#include <vector>
#include <string>
using namespace std;

#include "node.h"
#include "edge.h"
#include "graph.h"

/**
 * @brief An Implementation of the framework from Conte et al.
 * 
 * @param graph_filepath, the filepath of the graph to preform the algorithm on
 * @param cliques, a vector of cliques to be filled
 * 
 * 
 * 
 * @return int the size of the ECC
 */
int ecc_rc(string graph_filepath, vector<Node> &cliques) {

    return 0;
}


// vector<Edge>* find_clique_of (Graph G) {

//     return nullptr;
// } 

// string DATASET_PATH = "datasets/email-EuAll.txt";
string DATASET_PATH = "datasets/test1.txt";

int main() {
    Graph G(DATASET_PATH);
    return 0;
}