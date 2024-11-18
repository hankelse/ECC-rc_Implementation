#include <iostream>
#include <vector>
#include <string>
#include <chrono>

using namespace std;

#include "ecc.h"
#include "ecc-fs.h"
#include "ecc-cf.h"
#include "ecc-qec.h"
#include "ecc-qec1.h"
#include "ecc-red.h"
#include "ecc-nec.h"
#include "ecc-fr.h"

#include "io.h"
#include "node.h"
#include "edge.h"
#include "graph.h"
// #include "checks.h"
#include "connection.h"
#include "clique.h"
#include <gperftools/profiler.h>
// #include <gperftools/heap-checker.h>
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
    class_name.erase(0, 1); // typeid.name() starts with length of string
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
        cout << "\t\t " << cliques_found << " cliques  :  " << runtime << " milliseconds" << endl;

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
    chrono::steady_clock::time_point start_full_program = std::chrono::steady_clock::now();
    cout << "Running csv on all" << endl;

    vector<Graph*> graphs;

    vector<size_t> graph_edges;
    vector<size_t> graph_nodes;


    //Make datasets and save data
    cout << "Making Graphs" << endl;
    for (string filename : datasets) {
        cout << "\t making " << filename << endl;
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
    chrono::steady_clock::time_point end_full_program = chrono::steady_clock::now();
    size_t total_runtime = chrono::duration_cast<chrono::seconds>(end_full_program - start_full_program).count();
    cout << "Ran CSV_ON_ ALL in " << total_runtime << " seconds." << endl;

}


template<typename ECC_CLASS>
void get_csv_data_repeated (const vector<string>& datasets, vector<vector<double>>& all_algo_runtimes, vector<vector<double>>& all_algo_avg_num_cliques, vector<vector<double>>& all_algo_min_num_cliques, vector<vector<double>>& all_algo_max_num_cliques, vector<string>& algo_names, size_t repetitions) {
    string class_name = typeid(ECC_CLASS).name();
    class_name.erase(0, 1); // typeid.name() starts with length of string
    algo_names.push_back(class_name);
    cout << "COLLECTING DATA FOR: " << class_name << endl;

    vector<double> algo_runtimes;
    vector<double> algo_avg_num_cliques;
    vector<double> algo_min_num_cliques;
    vector<double> algo_max_num_cliques;
    for (string filename : datasets) {
        cout << "\tRunning on " << filename << " " << repetitions << " times."<< endl;

        size_t current_min_cliques = -1;
        size_t current_max_cliques = 0;
        size_t cliques_sum = 0;
        size_t runtime_sum = 0;
        for (int i = 0; i < repetitions; i++) {
            // Make ECC_CLASS object
        
            ECC_CLASS solver(filename);

            chrono::steady_clock::time_point start_time = std::chrono::steady_clock::now();

            size_t cliques_found = (*solver.run()).size();

            chrono::steady_clock::time_point end_time = chrono::steady_clock::now();

            size_t runtime = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();
            cout << "\t\t " << cliques_found << " cliques  :  " << runtime << " milliseconds" << endl;

            if (cliques_found < current_min_cliques || current_min_cliques == -1) {
                current_min_cliques = cliques_found;
            }
            if (cliques_found > current_max_cliques) {
                current_max_cliques = cliques_found;
            }

            cliques_sum += cliques_found;
            runtime_sum += runtime;


        }
        double average_runtime = static_cast<double>(runtime_sum) /repetitions;
        double average_cover_size = static_cast<double>(cliques_sum) / repetitions;
        algo_runtimes.push_back(average_runtime);
        algo_min_num_cliques.push_back(current_min_cliques);
        algo_max_num_cliques.push_back(current_max_cliques);
        algo_avg_num_cliques.push_back(average_cover_size);
    }
    cout << "COLLECTED ALL DATA FOR: " << class_name << endl;
    all_algo_runtimes.push_back(algo_runtimes);

    all_algo_avg_num_cliques.push_back(algo_avg_num_cliques);
    all_algo_min_num_cliques.push_back(algo_min_num_cliques);
    all_algo_max_num_cliques.push_back(algo_max_num_cliques);
    return;
}


template<typename... ECC_CLASSES>
void csv_on_all_repeated(const vector<string> datasets, const string csv_output_filepath, size_t repetitions) {
    chrono::steady_clock::time_point start_full_program = std::chrono::steady_clock::now();
    cout << "Running csv on all repeated" << endl;

    vector<Graph*> graphs;

    vector<size_t> graph_edges;
    vector<size_t> graph_nodes;


    //Make datasets and save data
    cout << "Making Graphs" << endl;
    for (string filename : datasets) {
        cout << "\t making " << filename << endl;
        Graph* graph = new Graph(filename);
        graphs.push_back(graph);
        graph_edges.push_back(graphs[graphs.size()-1]->_edges.size());
        graph_nodes.push_back(graphs[graphs.size()-1]->_nodes.size());
    }

    for (Graph* graph : graphs) {
        delete graph;
    }
    cout << "Finished Making Graphs" << endl;

    // Run algorithms and get result data
    vector<string> algo_names;
    vector<vector<double>> algo_runtimes;
    vector<vector<double>> algo_avg_num_cliques;
    vector<vector<double>> algo_min_num_cliques;
    vector<vector<double>> algo_max_num_cliques;

    // For each class, gets runs on all the graphs and appends data to lists
    (..., (get_csv_data_repeated<ECC_CLASSES>(datasets, algo_runtimes, algo_avg_num_cliques, algo_min_num_cliques, algo_max_num_cliques, algo_names, repetitions)));

    // //calculate min, max, and average for each algo and each dataset

    // for (int algo_index = 0; algo_index < algo_num_cliques.size() < algo_index++) {
    //     vector<size_t> clique_data = algo_num_cliques[algo_index]
    //     clique_min = 
    //     clique_sum = 0;
    // }


    // Call the CSV writing function
    data_to_csv_repeated(csv_output_filepath, datasets, graph_nodes, graph_edges, algo_names, algo_runtimes, algo_avg_num_cliques, algo_min_num_cliques, algo_max_num_cliques);

    
    chrono::steady_clock::time_point end_full_program = chrono::steady_clock::now();
    size_t total_runtime = chrono::duration_cast<chrono::seconds>(end_full_program - start_full_program).count();
    cout << "Ran CSV_ON_ ALL in " << total_runtime << " seconds." << endl;

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
"snap_datasets/p2p-Gnutella04.txt",
"snap_datasets/p2p-Gnutella05.txt", //[10]
"snap_datasets/p2p-Gnutella06.txt",
"snap_datasets/p2p-Gnutella08.txt",
"snap_datasets/p2p-Gnutella09.txt",
"snap_datasets/p2p-Gnutella24.txt", // [14]
"snap_datasets/p2p-Gnutella25.txt",
"snap_datasets/p2p-Gnutella30.txt",
"snap_datasets/p2p-Gnutella31.txt",
"snap_datasets/soc-Slashdot0811.txt", 
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
    "new_snap_datasets/web-Google.txt",
    "new_snap_datasets/web-NotreDame.txt",
    // "new_snap_datasets/web-Stanford.txt",
    // "new_snap_datasets/zhishi-hudong-internallink.edges"  
};

/* SETTINGS */

string const DATASET_PATH = "datasets/";
const char* PROFILER_OUT_PATH = "profile_output.prof";
string const CSV_OUT_PATH = "output.csv";

bool const DO_CHECKS = false;
bool const INCLUDE_BIG_DATA = false;




int main() {
    
    if (INCLUDE_BIG_DATA) {
        datasets.insert(datasets.end(), big_datasets.begin(), big_datasets.end());
    }

    

    // ProfilerStart(PROFILER_OUT_PATH);
    // Graph G(big_datasets[6]);

    // data_on_all<ECC_NEC>({datasets[0]});
    // data_on_all<ECC_NEC>({"snap_datasets/red_test.txt"});

    // ProfilerStop();

    // cout << removals << endl;
    // HeapLeakChecker checker("my_heap_check");
    // data_on_all<ECC_RED>({datasets[0]});

    // Node* test = new Node(0, 9);
    // test = nullptr;
    // datasets = {datasets[6]};


    // csv_on_all_repeated<ECC_NEC, ECC_FR>(datasets, CSV_OUT_PATH, 1);

    // csv_on_all<ECC_RED, ECC_NEC>(datasets, CSV_OUT_PATH);
    // csv_on_all_repeated<ECC_RED, ECC_NEC>(datasets, CSV_OUT_PATH, 5);
    // data_on_all<ECC_RED>(datasets);
    // csv_on_all_repeated<ECC_QEC1, ECC_RED>(datasets, CSV_OUT_PATH, 100);
    // csv_on_all_repeated<ECC_RED, ECC_QEC1>(datasets, CSV_OUT_PATH, 10);

    profile_on_all<ECC_FR>(datasets, PROFILER_OUT_PATH);

    // profile_on_all<ECC_QEC>(datasets, PROFILER_OUT_PATH);
    
    /*Profiling all the datasets */
    // profile_on_all<ECC>(datasets, PROFILER_OUT_PATH);

    // /* Getting data on all the datasets*/
    // data_on_all<ECC_QEC1> (datasets);

    // /* Looking at one in particular */
    // profile_on<ECC_FS>(datasets[8], PROFILER_OUT_PATH);


    return 0;
}



/*Running with gperf
Include path:
-I/opt/homebrew/include

Linked:
g++ -I/opt/homebrew/include -L/opt/homebrew/lib -lprofiler -std=c++17 -O3 *.cc -o ecc
g++ -I/opt/homebrew/include -L/opt/homebrew/lib -ltcmalloc_and_profiler -std=c++17 *.cc -o ecc
g++ -I/opt/homebrew/include -L/opt/homebrew/lib -std=c++17 -O3 -fsanitize=address *.cc -o ecc
g++ -fsanitize=address -L/opt/homebrew/lib -g -O1 -std=c++17 *.cc -o ecc

g++ -I/opt/homebrew/include -L/opt/homebrew/lib -lprofiler -std=c++17 -fsanitize=address -g *.cc -o ecc



See pprof
pprof ecc ./profile_output.prof  

pprof analysis
top100 --cum

Get call graph:
pprof --dot ./ecc profile_output.prof > output.dot
dot -Tpng output.dot -o output.png



all combined
g++ -I/opt/homebrew/include -L/opt/homebrew/lib -lprofiler -std=c++17 -O3 *.cc -o ecc; ./ecc; pprof --dot ./ecc profile_output.prof > output.dot; dot -Tpng output.dot -o output.png
*/