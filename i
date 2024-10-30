void data_to_csv(const string output_filepath,
                 const vector<string>& datasets, 
                 const vector<size_t>& dataset_node_nums, 
                 const vector<size_t>& dataset_edges_nums,
                 const vector<string>& algo_names,
                 const vector<vector<size_t>>& algo_runtimes, 
                 const vector<vector<size_t>>& algo_num_cliques) {
    
    ofstream csv_file(output_filepath);
    if (!csv_file.is_open()) {
        cerr << output_filepath << " could not be opened." << endl;
        exit(1);
    }
    //Format:| Dataset | Num Nodes | Num Edges | -- | Algorithm Run 1 |          |Algorithm Run 2 |         |...
    //       |         |           |           |    |  Cliques Found  | Runtime  |  Cliques found | Runtime |...
    
    // Write column headers to csv
    csv_file << "Dataset,|V|,|E|,,";
    for (string algo_name : algo_names) {
        csv_file << algo_name << ",,";
    }
    csv_file << "\n";

    //Row 2
    csv_file << ",,,,";
    for (int algo_index = 0; algo_index < algo_names.size(); algo_index++) {
        csv_file << "Cliques Found,Runtime,";
    }
    csv_file << "\n";

    //Data
    for (int dataset_index = 0; dataset_index < datasets.size(); dataset_index++) {

        // put in dataset info
        csv_file << datasets[dataset_index] << "," << dataset_node_nums[dataset_index] << "," << dataset_edges_nums[dataset_index] << ",,";

        //put in algorithm info
        for (int algo_index = 0; algo_index < algo_names.size(); algo_index++) {
            csv_file << algo_num_cliques[algo_index][dataset_index] << "," << algo_runtimes[algo_index][dataset_index] << ",";
        }
        csv_file << "\n";
    }
    csv_file.close();

}