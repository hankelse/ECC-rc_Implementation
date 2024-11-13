#include "io.h"

void data_to_csv(const string output_filepath,
                 const vector<string>& datasets, 
                 const vector<size_t>& dataset_node_nums, 
                 const vector<size_t>& dataset_edges_nums,
                 const vector<string>& algo_names,
                 const vector<vector<size_t>>& algo_runtimes, 
                 const vector<vector<size_t>>& algo_num_cliques) {
    
    string n = "\t"; // next column character

    ofstream csv_file(output_filepath);
    if (!csv_file.is_open()) {
        cerr << output_filepath << " could not be opened." << endl;
        exit(1);
    }
    //Format:| Dataset | Num Nodes | Num Edges | -- | Algorithm Run 1 |          |Algorithm Run 2 |         |...
    //       |         |           |           |    |  Cliques Found  | Runtime  |  Cliques found | Runtime |...
    
    // Write column headers to csv
    csv_file << "Dataset" << n << "Nodes in G" << n << "Edges in G" << n << n;
    for (string algo_name : algo_names) {
        csv_file << algo_name << n << n;
    }
    csv_file << "\n";

    //Row 2
    csv_file << n << n << n << n;
    for (int algo_index = 0; algo_index < algo_names.size(); algo_index++) {
        csv_file << "Cliques Found" << n << "Runtime" << n;
    }
    csv_file << "\n";

    //Data
    for (int dataset_index = 0; dataset_index < datasets.size(); dataset_index++) {

        // put in dataset info
        //uses the google sheets command =REGEXEXTRACT({string}, "[^/]+$") to only get the parts of {string} that occur after the last slash
        string dataset_clean = "=REGEXEXTRACT(\"" + datasets[dataset_index] + "\", \"[^/]+$\")";
        csv_file << dataset_clean << n << dataset_node_nums[dataset_index] << n << dataset_edges_nums[dataset_index] << n << n;

        //put in algorithm info
        for (int algo_index = 0; algo_index < algo_names.size(); algo_index++) {
            csv_file << algo_num_cliques[algo_index][dataset_index] << n << algo_runtimes[algo_index][dataset_index] << n;
        }
        csv_file << "\n";
    }
    csv_file.close();

}

void data_to_csv_repeated (const string output_filepath,
                 const vector<string>& datasets, 
                 const vector<size_t>& dataset_node_nums, 
                 const vector<size_t>& dataset_edges_nums,
                 const vector<string>& algo_names,
                 const vector<vector<double>>& algo_runtimes, 
                 const vector<vector<double>>& algo_avg_num_cliques,
                 const vector<vector<double>>& algo_min_num_cliques,
                 const vector<vector<double>>& algo_max_num_cliques) {
    
    string n = "\t"; // next column character

    ofstream csv_file(output_filepath);
    if (!csv_file.is_open()) {
        cerr << output_filepath << " could not be opened." << endl;
        exit(1);
    }
    //Format:| Dataset | Num Nodes | Num Edges | -- | Algorithm Run 1 |               |Algorithm Run 2 |         |...
    //       |         |           |           |    |  Avg Cliques Found | Min | Max | Runtime  |  Cliques found | Runtime |...
    
    // Write column headers to csv
    csv_file << "Dataset" << n << "Nodes in G" << n << "Edges in G" << n << n;
    for (string algo_name : algo_names) {
        csv_file << algo_name << n << n << n << n;
    }
    csv_file << "\n";

    //Row 2
    csv_file << n << n << n << n;
    for (int algo_index = 0; algo_index < algo_names.size(); algo_index++) {
        csv_file << "Average Cliques Found" << n << "Min Cliques Found" << n << "Max Cliques Found" << n << "Runtime" << n;
    }
    csv_file << "\n";

    //Data
    for (int dataset_index = 0; dataset_index < datasets.size(); dataset_index++) {

        // put in dataset info
        //uses the google sheets command =REGEXEXTRACT({string}, "[^/]+$") to only get the parts of {string} that occur after the last slash
        string dataset_clean = "=REGEXEXTRACT(\"" + datasets[dataset_index] + "\", \"[^/]+$\")";
        csv_file << dataset_clean << n << dataset_node_nums[dataset_index] << n << dataset_edges_nums[dataset_index] << n << n;

        //put in algorithm info
        for (int algo_index = 0; algo_index < algo_names.size(); algo_index++) {
            csv_file << algo_avg_num_cliques[algo_index][dataset_index] << n << algo_min_num_cliques[algo_index][dataset_index] << n << algo_max_num_cliques[algo_index][dataset_index] << n << algo_runtimes[algo_index][dataset_index] << n;
        }
        csv_file << "\n";
    }
    csv_file.close();

}

