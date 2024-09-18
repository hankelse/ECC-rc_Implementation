#include <iostream>
#include <vector>
#include <string>
using namespace std;

#include "io.h"
#include "node.h"
#include "edge.h"
#include "graph.h"
#include "checks.h"




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



vector<Edge>* find_clique_of (Graph G) {

    return nullptr;
} 

/**
 * @brief Runs checks from checks.h on G
 * 
 * @param G graph to run checks on
 */
void run_checks(Graph& G) {
    ostringstream warnings;
    bool passed = true;

    // If not all nodes are imported, print the warnings
    if (!all_nodes_imported(G, warnings)) { 
        cout << warnings.str() << endl;
        passed = false;
    }
    warnings.str(""); // empty

    // If not all edges are imported, print the warnings
    if (!all_edges_imported(G, warnings)) {
        cout << warnings.str() << endl;
        passed = false;
    }
    warnings.str(""); // empty

    if (passed) {
        cout << "\nPASSED ALL CHECKS\n" << endl;
    }
}


/* SETTINGS */
string DATASET_PATH = "datasets/email-EuAll.txt";
// string const DATASET_PATH = "datasets/test1.txt";
bool const DO_CHECKS = true;


int main() {
    //Collect graph
    Graph G(DATASET_PATH);

    //Run checks
    if (DO_CHECKS) { run_checks(G); }


    
    

    return 0;
}