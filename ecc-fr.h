/**
 * @file ecc-fr.h
 * @author Hank Elsesser
 * @brief A variation of the ECC-RED with fast reductions
 * @version 0.1
 * @date 2024-10-09
 * 
 * @copyright Copyright (c) 2024
 * 
 */


#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>

#include "ecc-nec.h"


class Graph;
class Node;
class Edge;
class Clique;
class Fast_set;

using namespace std;

#ifndef ECC_FR_H
#define ECC_FR_H

class ECC_FR : public ECC_NEC {
public:
    ECC_FR() = delete;                 
    ECC_FR(string dataset_filepath);   // Calls parent constructor + builds lookup set
    ECC_FR (Graph& Graph);

    //FROM NEC c_e
        // - Index i corresponds to the number of edges in the neighborhood of edge i
        // vector<size_t> edge_intersection_counts; 

    // New Datastructure: Index i corresponds to all of the common neighbors between edge i
    vector<vector<Node*>> common_neighbor_sets;

    virtual vector<Clique*>* run(); 
 
protected:


    //Overrides
    // vector<Node *> node_set_intersect(const vector<Node *>& vec_1, const vector<Node*>& vec_2);
    // void trim_candidates(std::vector<Node*>& candidates, const std::vector<Node*>& trimming_vec);
    // Node* extract_node(vector<Node*>& potential_additions, Clique* clique);
    virtual Clique* find_clique_of(Edge* edge);

    virtual size_t apply_rule_one();
    virtual size_t apply_rule_two();


    // virtual Edge* select_uncovered_edge(int& previous_index);
    // virtual void init_edge_intersection_counts();


    // New Methods
    void find_all_common_neighbors(); // For each edge, finds and stores all neighbors of both of its nodes
    vector<Node*> get_common_neighbors(Edge*); // Returns the common neighbors of the nodes in an edge

    

};



#endif