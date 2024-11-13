#include <iostream>
#include <fstream>

using namespace std;
#include <type_traits>
#include <vector>
#include <string>

#ifndef IO_H
#define IO_H

/**
 * @brief Overloading << for all types of vectors. 
 * If the type is a pointer, the dereferenced value is printed.
 * 
 * @tparam T  the datatype in the vector
 * @param os  the output stream
 * @param v  the vector to print
 * @return ostream& 
 */
template <typename T>
ostream& operator<<(ostream &os, const vector<T> &v) {
    // Check if vector is empty
    if (v.size() < 1) {
        os << "[vector is empty]";
    } else {

        //Check if T is a pointer
        if constexpr (is_pointer<T>::value) {

            //send DEREFERENCED entries to os
            os << "[" << *v[0]; //no ' ,' before first
            for (int i = 1; i < v.size(); i++) {
                os << ", " << *v[i];
            }
            os << "]";

        } else {

        // send entries to os AS IS
            os << "[" << v[0]; //no ' ,' before first
            for (int i = 1; i < v.size(); i++) {
                os << ", " << v[i];
            }
            os << "]";

        }
    }
    return os;
}


void data_to_csv(const string output_filepath,
                 const vector<string>& datasets, 
                 const vector<size_t>& dataset_node_nums, 
                 const vector<size_t>& dataset_edges_nums,
                 const vector<string>& algo_names,
                 const vector<vector<size_t>>& algo_runtimes, 
                 const vector<vector<size_t>>& algo_num_cliques);

void data_to_csv_repeated (const string output_filepath,
                 const vector<string>& datasets, 
                 const vector<size_t>& dataset_node_nums, 
                 const vector<size_t>& dataset_edges_nums,
                 const vector<string>& algo_names,
                 const vector<vector<double>>& algo_runtimes, 
                 const vector<vector<double>>& algo_avg_num_cliques,
                 const vector<vector<double>>& algo_min_num_cliques,
                 const vector<vector<double>>& algo_max_num_cliques);

#endif