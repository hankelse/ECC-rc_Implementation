/**
 * @file ecc-cf : ECC Covering Frequencies
 * @author Hank Elsesser
 * @brief An extension of ECC-FS that keeps track of the number of times that edges are put into cliques to speed up covering.
 * @version 0.1
 * @date 2024-10-21
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include "ecc-fs.h"


class Graph;
class Node;
class Edge;
class Clique;
class Fast_set;

using namespace std;

#ifndef ECC_CF_H
#define ECC_CF_H

class ECC_CF : public ECC_FS {
public:
    ECC_CF() = delete;                 //No default constructor
    ECC_CF(string dataset_filepath); // calls parent constructor 

    Fast_set* candidates_fs;

    //NEW: Frequency vector for number of cliques each edge is in
    vector<size_t> clique_counts;

    //Overriding run to not use num_edges_covered
    virtual vector<Clique*>* run();         //Runs the algorithm Returns a pointer to the clique_cover

    //Overriding due to edge.covered not being used
    virtual bool check_solution();
    virtual void check_solution_debug();

protected:

    // Methods that are being overridden
    virtual Edge* select_uncovered_edge(int& previous_index);
    virtual Clique* find_clique_of(Edge* edge);
    virtual Node* extract_node(vector<Node*>& candidate_set, Clique* clique);






};



#endif