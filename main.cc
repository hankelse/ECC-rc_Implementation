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
int ecc_rc(Graph& G, vector<vector<Edge*>> &cliques) {
    

    //while there are uncovered edges do
    int num_edges_covered = 0;
    int last_uncovered_edge_index = 0;

    while (num_edges_covered < G._num_edges) {
        // u, v ← SELECT UNCOVERED EDGE()
        Edge* uncovered_edge = G.select_uncovered_edge(last_uncovered_edge_index);

        uncovered_edge->cover();
        cout << *uncovered_edge << endl;
        num_edges_covered +=1;
        //R ← FIND CLIQUE OF(u, v)
        


    }


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
    int num_tests_failed = 0;

    cout << "\tRUNNING ALL CHECKS" << endl;

    // If not all nodes are imported, print the warnings
    if (!all_nodes_imported(G, warnings)) { 
        cout << warnings.str() << endl;
        num_tests_failed += 1;
    } else {
        cout << "\t\tPASSED all_nodes_imported" << endl;
    }
    warnings.str(""); // empty


    // If not all edges are imported, print the warnings
    if (!all_edges_imported(G, warnings)) {
        cout << warnings.str() << endl;
        num_tests_failed += 1;
    } else {
        cout << "\t\tPASSED all_edges_imported" << endl;
    }
    warnings.str(""); // empty


    // If adjacency lists don't match manual lists, print the warnigns
    if (!adj_lists_correct(G, warnings)) {
        cout << warnings.str() << endl;
        num_tests_failed += 1;
    } else {
        cout << "\t\tPASSED adj_lists_correct" << endl;
    }
    warnings.str(""); // empty


    if (!num_tests_failed) {
        cout << "\tPASSED ALL CHECKS\n" << endl;
    }  else {
        cout << "\tFAILED " << num_tests_failed << " TESTS.\n" << endl;
    }
}




/* SETTINGS */
// string DATASET_PATH = "datasets/email-EuAll.txt";
string const DATASET_PATH = "datasets/test1.txt";
bool const DO_CHECKS = true;


int main() {
    //Collect graph
    Graph G(DATASET_PATH);

    //Run checks
    if (DO_CHECKS) { run_checks(G); }

    //run ecc_rc
    vector<vector<Edge*>> clique_cover;
    ecc_rc(G, clique_cover);

    

 


    return 0;
}

    // cout << "Neighbors and Edges:" << endl;
    // for (int i = 0; i < G._nodes.size(); i++) {
    //     cout << "\tNode "<< i << ":" << endl;
    //     cout << "\t\t Neighbors: " << G._nodes[i]->neighbors << endl; 
    //     cout << "\t\t Edges: " << G._nodes[i]->edges << endl; 
    //     cout << "ALSO... done the other way..." << endl;
    //     vector<Edge*> containing_edges = G._nodes[0]->get_edges(G);
    //     vector<Node*> neighboring_nodes = G._nodes[0]->get_neighbors(G);
    //     cout << "\t\t Neighbors: " << neighboring_nodes << endl; 
    //     cout << "\t\t Edges: " << containing_edges << endl;  
    // }

    // // -- Reading in Data Demo -- //
    // cout << "The nodes in G are " << G._nodes << endl;
    // cout << "The edges in G are " << G._edges << endl;
    // cout << "\n" << endl;
    // cout << "Adjacency Lists:" << endl;
    // for (int i = 0; i < G._nodes.size(); i++ ) {
    //     cout << "\t The neighbors of " << *G._nodes[i] << " are " << G._nodes[i]->neighbors << endl;
    // }
    // cout << "\n";

    // // -- Overloading Operators Demo -- //
    
    // Node node = Node(45);
    // cout << "1) Printing a node: " << node << endl << endl;

    // Edge edge = Edge(&node, &node);
    // cout << "2) Printing an edge: " << edge << endl << endl;

    // vector<Node> nodes = {Node(10), Node(20), Node(30)};
    // cout << "3) Printing a vector of Nodes: " << nodes << endl << endl;

    // vector<Edge*> edges = {new Edge(&nodes[0], &nodes[1]), new Edge(&nodes[1], &nodes[2])};
    // cout << "4) Printing a vector of Edge*s: " << edges << endl << endl;