#include <iostream>
#include <vector>
#include <string>
#include <ctime>

using namespace std;

#include "io.h"
#include "node.h"
#include "edge.h"
#include "graph.h"
#include "checks.h"
#include "connection.h"
#include "clique.h"



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
size_t demo_ecc_rc(Graph& G, vector<Clique*> &cliques) {
    cout << "\n\n\t\tSTARTING ECC-rc\n\n" << endl;
 
    //while there are uncovered edges do
    size_t num_edges_covered = 0;
    int last_uncovered_edge_index = 0;

    while (num_edges_covered < G._num_edges) {

        // u, v ← SELECT UNCOVERED EDGE()
        Edge* uncovered_edge = G.select_uncovered_edge(last_uncovered_edge_index);
        cout << "1) SELECTED UNCOVERED EDGE: \t" << *uncovered_edge << endl;

        //R ← FIND CLIQUE OF(u, v)
        cout << "2) FINDING CLIQUE OF \t" << *uncovered_edge << endl;
        Clique* found_clique = G.demo_find_clique_of(uncovered_edge, num_edges_covered);
        cout << "   => FOUND CLIQUE OF " <<  *uncovered_edge << ": \t" << *found_clique << endl;

        // C ← C∪R
        cliques.push_back(found_clique);
        cout << "3) ADDED CLIQUE TO ECC \t num_cliques = " << cliques.size() << endl;

        // Done?
        cout << "4) COVERED " << num_edges_covered << " / " << G._num_edges;
        if (num_edges_covered < G._num_edges) {
            cout << " -> REPEATING \n\n" << endl;
        } else {
            cout << " -> FINISHED\n" << endl;
        }
    }


    return cliques.size();
}

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
size_t ecc_rc(Graph& G, vector<Clique*> &cliques) {
 
    //while there are uncovered edges do
    size_t num_edges_covered = 0;
    int last_uncovered_edge_index = 0;

    while (num_edges_covered < G._num_edges) {

        // u, v ← SELECT UNCOVERED EDGE()
        Edge* uncovered_edge = G.select_uncovered_edge(last_uncovered_edge_index);

        //R ← FIND CLIQUE OF(u, v)
        Clique* found_clique = G.find_clique_of(uncovered_edge, num_edges_covered);

        // C ← C∪R
        cliques.push_back(found_clique);


    }
    return cliques.size();
}

vector<Edge>* find_clique_of (Graph G) {

    return nullptr;
} 

/**
 * @brief Runs checks from checks.h on G
 * 
 * @param G graph to run checks on
 */
bool run_checks(Graph& G) {
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
        return true;
    }
    cout << "\tFAILED " << num_tests_failed << " TESTS.\n" << endl;
    return false;
}

/**
 * @brief Is the Clique Cover legitimate?
 * Checks to make sure all edges have been covered.
 *
 * @param G 
 * @param clique_cover 
 * @return 
 */
bool check_ECC (Graph& G, vector<Clique*>& clique_cover) {
    for (Edge* edge : G._edges) {
        if (edge->is_covered() == false) {
            return false;
        }
    }
    return true;
}


void run_on(string filename) {

    cout << "\nRunning ECC on " << filename << "." << endl;
    time_t ecc_start = time(NULL);
    Graph G(filename);

    cout << "\tNodes: " << G._num_nodes << endl;
    cout << "\tEdges: " << G._num_edges << endl;

    vector<Clique*> clique_cover;
    size_t k = ecc_rc(G, clique_cover);

    time_t ecc_end = time(NULL);

    double runtime = difftime(ecc_end, ecc_start);

    if (check_ECC(G, clique_cover)) {
        cout << "Algorithm found cover of G with \n\t" << k << " cliques \n\tin " << runtime << " seconds.\n" << endl;
    } else {
        cout << "Algorithm did not cover all edges." << endl;
    }
    


}

void run_on_demo(string filename) {

    cout << "\nRunning ECC demo on " << filename << "." << endl;
    time_t ecc_start = time(NULL);
    Graph G(filename);

    cout << "\tNodes: " << G._num_nodes << endl;
    cout << "\tEdges: " << G._num_edges << endl;

    vector<Clique*> clique_cover;
    size_t k = demo_ecc_rc(G, clique_cover);

    time_t ecc_end = time(NULL);

    double runtime = difftime(ecc_end, ecc_start);
    cout << "Algorithm found cover of G with \n\t" << k << " cliques \n\tin " << runtime << " seconds.\n" << endl;
}


/* SETTINGS */
// string const DATASET_PATH = "datasets/clique.txt";
// string const DATASET_PATH = "datasets/test1.txt";
// string const DATASET_PATH = "datasets/email-EuAll.txt";
// string const DATASET_PATH = "datasets/email-Enron.txt";
// string const DATASET_PATH = "datasets/soc-Slashdot0811.txt";
string const DATASET_PATH = "datasets/wiki-Vote.txt";
bool const DO_CHECKS = true;


vector<string> datasets = {
"datasets/ca-AstroPh.txt", 
"datasets/ca-CondMat.txt", 
"datasets/ca-GrQc.txt", 
"datasets/ca-HepPh.txt", 
"datasets/ca-HepTh.txt", 
"datasets/cit-HepTh.txt",
"datasets/cit-HepPh.txt",
"datasets/email-Enron.txt",
"datasets/email-EuAll.txt",
"datasets/p2p-Gnutella31.txt",
"datasets/soc-Slashdot0811.txt",
"datasets/soc-Slashdot0902.txt",
"datasets/wiki-Vote.txt",
};

int main() {


    // string filename = "datasets/demo1.txt";
    // run_on_demo(filename);



    if (DO_CHECKS) {
    
        vector<string> properly_imported;
        vector<string> improperly_imported;


        for (string filepath : datasets) {
            Graph G(filepath);
            if(run_checks(G)) {
                cout << filepath << "is being imported correctly!" << endl;
                properly_imported.push_back(filepath);
                continue;
            }
            cout << filepath << "is NOT being imported correctly." << endl;
            improperly_imported.push_back(filepath);
        }

        cout << "Properly Imported: "  << endl;
        for (string filename : properly_imported) {
            cout << "\t - " << filename << endl;
        }

        cout << "NOT Properly Imported: "  << endl;
        for (string filename : improperly_imported) {
            cout << "\t - " << filename << endl;
        }
        datasets = properly_imported;
    }


    for (string filepath : datasets) {
        run_on(filepath);
    }






     // time_t ecc_start = time(NULL);

    //Collect graph
    Graph G(DATASET_PATH);

    // //Run checks
    if (DO_CHECKS) { run_checks(G); cout << "finished " << endl;}
    
    // vector<Clique*> clique_cover;
    // size_t k = demo_ecc_rc(G, clique_cover);

    // // //run ecc_rc
    // // vector<Clique*> clique_cover;
    // // size_t k = ecc_rc(G, clique_cover);

    // // time_t ecc_end = time(NULL);

    // // double runtime = difftime(ecc_end, ecc_start);
    // // cout << "Algorithm found cover of G with \n\t" << k << " cliques \n\tin " << runtime << "seconds." << endl;



    // // run_on ("datasets/clique.txt");
    // // run_on ("datasets/test1.txt");
    run_on ("datasets/email-EuAll.txt");
    run_on ("datasets/soc-Slashdot0811.txt");
    run_on ("datasets/soc-Slashdot0902.txt");
    run_on ("datasets/email-Enron.txt");
    // run_on ("datasets/wiki-Vote.txt");

    


    // run_on ("");
    

    // run_on_demo(DATASET_PATH);

    

 

    return 0;
}