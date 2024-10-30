#include <iostream>
#include <vector>
#include <string>
#include <chrono>

using namespace std;

#include "ecc.h"
#include "ecc-fs.h"
#include "ecc-cf.h"
#include "ecc-qec.h"

#include "io.h"
#include "node.h"
#include "edge.h"
#include "graph.h"
// #include "checks.h"
#include "connection.h"
#include "clique.h"
#include <gperftools/profiler.h>
#include <chrono>
#include <thread>



// /* 

// // /**
// //  * @brief Runs checks from checks.h on G
// //  * 
// //  * @param G graph to run checks on
// //  */
// bool run_checks(Graph& G) {
//     ostringstream warnings;
//     int num_tests_failed = 0;

//     cout << "\tRUNNING ALL CHECKS" << endl;

//     // If not all nodes are imported, print the warnings
//     if (!all_nodes_imported(G, warnings)) { 
//         cout << warnings.str() << endl;
//         num_tests_failed += 1;
//     } else {
//         cout << "\t\tPASSED all_nodes_imported" << endl;
//     }
//     warnings.str(""); // empty


//     // If not all edges are imported, print the warnings
//     if (!all_edges_imported(G, warnings)) {
//         cout << warnings.str() << endl;
//         num_tests_failed += 1;
//     } else {
//         cout << "\t\tPASSED all_edges_imported" << endl;
//     }
//     warnings.str(""); // empty


//     // If adjacency lists don't match manual lists, print the warnigns
//     if (!adj_lists_correct(G, warnings)) {
//         cout << warnings.str() << endl;
//         num_tests_failed += 1;
//     } else {
//         cout << "\t\tPASSED adj_lists_correct" << endl;
//     }
//     warnings.str(""); // empty


//     if (!num_tests_failed) {
//         cout << "\tPASSED ALL CHECKS\n" << endl;
//         return true;
//     }
//     cout << "\tFAILED " << num_tests_failed << " TESTS.\n" << endl;
//     return false;
// }

//  */

/**
 * @brief Runs full algorithm on a dataset. Measures running time and checks correctness.
 * @param filename the filename of the dataset to run on.
 */
template<typename ECC_CLASS>
void profile_on(string filename, const char* profile_output_path) {

    ECC_CLASS solver(filename);
    const Graph* G = solver.graph();



    //start timer
    chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();
    ProfilerStart(profile_output_path);

    //run algorithm
    vector<Clique*>* clique_cover = solver.run();

    ProfilerStop();
    size_t k = clique_cover->size();
    cout << "found " << k << " cliques" << endl;

    //Get the time passed in miliseconds
    chrono::steady_clock::time_point end_time = chrono::steady_clock::now();
    size_t runtime = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();


    if (solver.check_solution()) {
        cout << "Algorithm found cover of G from: "<< filename <<" with \n\t" << k << " cliques \n\tin " << runtime << " miliseconds.\n" << endl;
    } else {
        cout << "Algorithm did not cover all edges." << endl;
    }



}


/**
 * @brief Runs algo on all datasets, then prints copyable data sequences for copy and paste to table
 * 
 * @param datasets 
 */
template<typename ECC_CLASS>
void data_on_all(vector<string> datasets) {
    vector<size_t> clique_stats;
    vector<size_t> time_stats;
    vector<size_t> edge_stats;
    vector<size_t> node_stats;

    for (string filename : datasets) {
        //make graph
        ECC_CLASS solver(filename);
        const Graph* G = solver.graph();

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
            solver.check_solution_debug();
            cin.get();
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


template<typename ECC_CLASS>
void get_csv_data(const vector<string>& datasets, vector<vector<size_t>>& all_algo_runtimes, vector<vector<size_t>>& all_algo_num_cliques, vector<string>& algo_names) {

    string class_name = typeid(ECC_CLASS).name();
    algo_names.push_back(class_name);
    cout << "COLLECTING DATA FOR: " << class_name << endl;

    vector<size_t> algo_runtimes;
    vector<size_t> algo_num_cliques;
    for (string filename : datasets) {
        // Make ECC_CLASS object
        cout << "\tRunning on " << filename << endl;
        ECC_CLASS solver(filename);

        chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();

        size_t cliques_found = (*solver.run()).size();

        chrono::steady_clock::time_point end_time = chrono::steady_clock::now();

        size_t runtime = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
        cout << "\t\t Found " << cliques_found << " cliques." << endl;
        cout << "\t\t In    " << runtime << " miliseconds." <<endl;

        algo_runtimes.push_back(runtime);
        algo_num_cliques.push_back(cliques_found);
    }
    cout << "COLLECTED ALL DATA FOR: " << class_name << endl;
    all_algo_runtimes.push_back(algo_runtimes);
    all_algo_num_cliques.push_back(algo_num_cliques);
    return;
}

//https://en.cppreference.com/w/cpp/language/parameter_pack
template<typename... ECC_CLASSES>
void csv_on_all(const vector<string> datasets, const string csv_output_filepath) {
    cout << "Running csv on all" << endl;

    vector<Graph*> graphs;

    vector<size_t> graph_edges;
    vector<size_t> graph_nodes;


    //Make datasets and save data
    cout << "Making Graphs" << endl;
    for (string filename : datasets) {
        Graph* graph = new Graph(filename);
        graphs.push_back(graph);
        graph_edges.push_back(graphs[graphs.size()-1]->_edges.size());
        graph_nodes.push_back(graphs[graphs.size()-1]->_nodes.size());
    }
    cout << "Finished Making Graphs" << endl;

    // Run algorithms and get result data
    vector<string> algo_names;
    vector<vector<size_t>> algo_runtimes;
    vector<vector<size_t>> algo_num_cliques;

    // For each class, gets runs on all the graphs and appends data to lists
    (..., (get_csv_data<ECC_CLASSES>(datasets, algo_runtimes, algo_num_cliques, algo_names)));


    // Call the CSV writing function
    data_to_csv(csv_output_filepath, datasets, graph_nodes, graph_edges, algo_names, algo_runtimes, algo_num_cliques);

    for (Graph* graph : graphs) {
        delete graph;
    }

}


template<typename ECC_CLASS>
void profile_on_all(vector<string> datasets, const char* profile_output_path) {
    //Make all objects before starting profile
    cout << "Building solver objects" << endl;
    vector<ECC_CLASS> solvers;
    for (string dataset : datasets) {
        cout << "\t " << dataset << endl;
        solvers.push_back(ECC_CLASS(dataset));
    }

    cout << "Starting profile..." << endl;
    ProfilerStart(profile_output_path);
    for (ECC_CLASS solver : solvers) {
        cout << "Running on " << solver.dataset_filepath << endl;
        cout << "\t->" << solver.run()->size() << " cliques" << endl;;
    }
    ProfilerStop();
    cout << "Profile finished." << endl;

}


vector<string> datasets = {
"snap_datasets/ca-AstroPh.txt", 
"snap_datasets/ca-CondMat.txt", 
"snap_datasets/ca-GrQc.txt", 
"snap_datasets/ca-HepPh.txt", 
"snap_datasets/ca-HepTh.txt", 
"snap_datasets/cit-HepPh.txt", //[5]
"snap_datasets/cit-HepTh.txt",
"snap_datasets/email-Enron.txt",
"snap_datasets/email-EuAll.txt", //[8]
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

/* SETTINGS */

string const DATASET_PATH = "datasets/";
const char* PROFILER_OUT_PATH = "profile_output.prof";
string const CSV_OUT_PATH = "output.csv";

bool const DO_CHECKS = false;
bool const INCLUDE_BIG_DATA = false;
// class ECC_class = ECC;


int main() {
    
    if (INCLUDE_BIG_DATA) {
        datasets.insert(datasets.end(), big_datasets.begin(), big_datasets.end());
    }

    // ECC_QEC solver(datasets[0]);
    // cout << "Initialize object" << endl;
    // const Graph* G = solver.graph();
    // cout << "Got Graph" << endl;
    // cout << "Pre-Run" << endl;
    // cout << (*solver.run()).size() << endl;
    // cout << G -> _edges << endl;

    // datasets = {datasets[0], datasets[1]};

    csv_on_all<ECC, ECC_FS, ECC_CF, ECC_QEC>(datasets, CSV_OUT_PATH);

    // profile_on<ECC_FS>(datasets[0], PROFILER_OUT_PATH);


    // profile_on_all<ECC_CF>(datasets, PROFILER_OUT_PATH);
    // solver.run();
    // solver.check_solution_debug();
    
    /*Profiling all the datasets */
    // profile_on_all<ECC_CF>(datasets, PROFILER_OUT_PATH);
    // profile_on_all<ECC>(datasets, PROFILER_OUT_PATH);

    // /* Getting data on all the datasets*/
    // data_on_all<ECC_FS> (datasets);
    // cin.get();
    // data_on_all<ECC_CF> (datasets);
    // data_on_all<ECC_FS> (datasets);
    // data_on_all<ECC> (datasets);

    // /* Looking at one in particular */
    // profile_on<ECC_FS>(datasets[8], PROFILER_OUT_PATH);


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

Get call graph:
pprof --dot ./ecc profile_output.prof > output.dot
dot -Tpng output.dot -o output.png


all combined
g++ -I/opt/homebrew/include -L/opt/homebrew/lib -lprofiler -std=c++17 *.cc -o ecc; ./ecc; pprof --dot ./ecc profile_output.prof > output.dot; dot -Tpng output.dot -o output.png
*/