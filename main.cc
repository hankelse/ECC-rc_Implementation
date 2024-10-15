#include <iostream>
#include <vector>
#include <string>
#include <chrono>

using namespace std;

#include "ecc.h"
#include "io.h"
#include "node.h"
#include "edge.h"
#include "graph.h"
#include "checks.h"
#include "connection.h"
#include "clique.h"
#include <gperftools/profiler.h>


// /**
//  * @brief An Implementation of the framework from Conte et al.
//  * 
//  * @param graph_filepath, the filepath of the graph to preform the algorithm on
//  * @param cliques, a vector of cliques to be filled
//  * 
//  * 
//  * 
//  * @return int the size of the ECC
//  */
// size_t ecc_rc(Graph& G, vector<Clique*> &cliques) {
 
//     //while there are uncovered edges do
//     size_t num_edges_covered = 0;
//     int last_uncovered_edge_index = 0;

//     while (num_edges_covered < G._num_edges) {

//         // u, v ← SELECT UNCOVERED EDGE()
//         Edge* uncovered_edge = G.select_uncovered_edge(last_uncovered_edge_index);

//         //R ← FIND CLIQUE OF(u, v)
//         Clique* found_clique = G.find_clique_of(uncovered_edge, num_edges_covered);

//         // C ← C∪R
//         cliques.push_back(found_clique);


//     }
//     return cliques.size();
// }

// vector<Edge>* find_clique_of (Graph G) {

//     return nullptr;
// } 

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

// /**
//  * @brief Is the Clique Cover legitimate?
//  * Checks to make sure all edges have been covered.
//  *
//  * @param G 
//  * @param clique_cover 
//  * @return 
//  */
// bool check_ECC (Graph& G, vector<Clique*>& clique_cover) {
//     for (Edge* edge : G._edges) {
//         if (!edge->is_covered()) {
//             return false;
//         }
//     }
//     return true;
// }

// /**
//  * @brief Runs full algorithm on a dataset. Measures running time and checks correctness.
//  * @param filename the filename of the dataset to run on.
//  */
// void run_on(string filename) {

//     cout << "\nRunning ECC on " << filename << "." << endl;
    
//     //Save the starting time of the algorithm
//     chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();

//     Graph G(filename);

//     cout << "\tNodes: " << G._num_nodes << endl;
//     cout << "\tEdges: " << G._num_edges << endl;

//     vector<Clique*> clique_cover;
//     size_t k = ecc_rc(G, clique_cover);

//     //Get the time passed in miliseconds
//     chrono::steady_clock::time_point end_time = chrono::steady_clock::now();
//     size_t runtime = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();


//     if (check_ECC(G, clique_cover)) {
//         cout << "Algorithm found cover of G with \n\t" << k << " cliques \n\tin " << runtime << " miliseconds.\n" << endl;
//     } else {
//         cout << "Algorithm did not cover all edges." << endl;
//     }
    


// }

// /**
//  * @brief Runs full algorithm on a dataset. Measures running time and checks correctness.
//  * @param filename the filename of the dataset to run on.
//  */
// void run_on_profile(string filename) {

//     cout << "\nRunning ECC on " << filename << "." << endl;
    


//     Graph G(filename);
    
//     //Save the starting time of the algorithm
//     chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();

//     cout << "\tNodes: " << G._num_nodes << endl;
//     cout << "\tEdges: " << G._num_edges << endl;

//     ProfilerStart("profile_output.prof");

//     vector<Clique*> clique_cover;
//     size_t k = ecc_rc(G, clique_cover);

//     ProfilerStop();

//     //Get the time passed in miliseconds
//     chrono::steady_clock::time_point end_time = chrono::steady_clock::now();
//     size_t runtime = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
    


//     if (check_ECC(G, clique_cover)) {
//         cout << "Algorithm found cover of G with \n\t" << k << " cliques \n\tin " << runtime << " miliseconds.\n" << endl;
//     } else {
//         cout << "Algorithm did not cover all edges." << endl;
//     }
    


// }


// // void run_on_demo(string filename) {

// //     cout << "\nRunning ECC demo on " << filename << "." << endl;
// //     time_t ecc_start = time(NULL);
// //     Graph G(filename);

// //     cout << "\tNodes: " << G._num_nodes << endl;
// //     cout << "\tEdges: " << G._num_edges << endl;

// //     vector<Clique*> clique_cover;
// //     size_t k = demo_ecc_rc(G, clique_cover);

// //     time_t ecc_end = time(NULL);

// //     double runtime = difftime(ecc_end, ecc_start);
// //     cout << "Algorithm found cover of G with \n\t" << k << " cliques \n\tin " << runtime << " seconds.\n" << endl;
// // }

// /**
//  * @brief Runs run_on on a list of filenames
//  * If do_checks, first checks if all the data is imported correctly, 
//  * then runs on correctly imported data.
//  * 
//  * @param datasets list of datasets to run on
//  * @param do_checks bool for if graphs should be checked
//  */
// void run_on_all(vector<string> datasets, bool do_checks) {
//     //Run checks on each dataset and then report and exclude ones that don't pass
//     if (do_checks) {
//         vector<string> properly_imported;
//         vector<string> improperly_imported;

//         //Check whether or not dataset was properly imported and put in corresponding list
//         for (string filepath : datasets) {
//             Graph G(filepath);
//             if(run_checks(G)) {
//                 cout << filepath << " is being imported correctly!" << endl;
//                 properly_imported.push_back(filepath);
//                 continue;
//             }
//             cout << filepath << " is NOT being imported correctly." << endl;
//             improperly_imported.push_back(filepath);
//         }

//         //Report the results
//         cout << "Properly Imported: "  << endl;
//         for (string filename : properly_imported) {
//             cout << "\t - " << filename << endl;
//         }

//         cout << "NOT Properly Imported: "  << endl;
//         for (string filename : improperly_imported) {
//             cout << "\t - " << filename << endl;
//         }
//         if (improperly_imported.size() == 0) {
//             cout << "\t None!" << endl;
//         }
//         datasets = properly_imported;

//         cout << "\nPress ENTER to continue: Will run algorithm on " << properly_imported.size() << " properly imported graphs." << endl;
//         cin.get();
//      }


//     for (string filepath : datasets) {
//         run_on(filepath);
//     } 
// }

/**
 * @brief Runs algo on all datasets, then prints copyable data sequences for copy and paste to table
 * 
 * @param datasets 
 */
void data_on_all(vector<string> datasets) {
    vector<size_t> clique_stats;
    vector<size_t> time_stats;
    vector<size_t> edge_stats;
    vector<size_t> node_stats;

    for (string filename : datasets) {
        //make graph
        ECC solver(filename);
        Graph* G = solver.graph();

        edge_stats.push_back(G->_edges.size());
        node_stats.push_back(G->_nodes.size());


        //start timer
        chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();

        //run algorithm
        vector<Clique*>* clique_cover = solver.run();
        size_t k = clique_cover->size();

        //Get the time passed in miliseconds
        chrono::steady_clock::time_point end_time = chrono::steady_clock::now();
        size_t runtime = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();


        if (solver.check_solution()) {
            cout << "Algorithm found cover of G from: "<< filename <<" with \n\t" << k << " cliques \n\tin " << runtime << " miliseconds.\n" << endl;
            clique_stats.push_back(k);
            time_stats.push_back(runtime);
        } else {
            cout << "Algorithm did not cover all edges." << endl;
            clique_stats.push_back(-1);
            time_stats.push_back(-1);
        }
    }
    cout << "\nDATA for copy/paste!\n" << endl;
    cout << "\nDataset names:" << endl;
    for (int i = 0; i < datasets.size(); i++) {
        cout << datasets[i] << endl;
    }
    cout << "\nDataset nodes:" << endl;
    for (int i = 0; i < datasets.size(); i++) {
        cout << node_stats[i] << endl;
    }
    cout << "\nDataset edges:" << endl;
    for (int i = 0; i < datasets.size(); i++) {
        cout << edge_stats[i] << endl;
    }
    cout << "\nCliques:" << endl;
    for (int i = 0; i < datasets.size(); i++) {
        cout << clique_stats[i] << endl;
    }
    cout << "\nTimes:" << endl;
    for (int i = 0; i < datasets.size(); i++) {
        cout << time_stats[i] << endl;
    }

}

/* SETTINGS */

string const DATASET_PATH = "datasets/";
bool const DO_CHECKS = false;
bool const INCLUDE_BIG_DATA = true;
string const PROFILER_OUT_PATH = "profile_output.prof";


vector<string> datasets = {
"snap_datasets/ca-AstroPh.txt", 
"snap_datasets/ca-CondMat.txt", 
"snap_datasets/ca-GrQc.txt", 
"snap_datasets/ca-HepPh.txt", 
"snap_datasets/ca-HepTh.txt", 
"snap_datasets/cit-HepPh.txt", //[5]
"snap_datasets/cit-HepTh.txt",
"snap_datasets/email-Enron.txt",
"snap_datasets/email-EuAll.txt",
"snap_datasets/p2p-Gnutella31.txt",
"snap_datasets/soc-Slashdot0811.txt", //[10]
"snap_datasets/soc-Slashdot0902.txt",
"snap_datasets/wiki-Vote.txt",
};

vector<string> big_datasets = {
    "new_snap_datasets/amazon0302.txt", 
    "new_snap_datasets/amazon0312.txt", 
    "new_snap_datasets/roadNet-CA.txt", 
    "new_snap_datasets/roadNet-PA.txt", 
    "new_snap_datasets/roadNet-TX.txt", 
    // "new_snap_datasets/web-BerkStan.txt", 
    "new_snap_datasets/web-Google.txt"
};

string dataset = "snap_datasets/test1.txt";

int main() {
    
    if (INCLUDE_BIG_DATA) {
        datasets.insert(datasets.end(), big_datasets.begin(), big_datasets.end());
    }
    
    ProfilerStart("profile_output.prof");


    data_on_all(datasets);

    ProfilerStop();

    return 0;
}

/*Running with gperf
Include path:
-I/opt/homebrew/include

Linked:
g++ -I/opt/homebrew/include -L/opt/homebrew/lib -lprofiler -std=c++17 *.cc -o ecc

See pprof
pprof ecc ./profile_output.prof  

pprof analysis
top100 --cum


*/