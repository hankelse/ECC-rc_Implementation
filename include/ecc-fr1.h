/**
 * @file ecc-fr.h
 * @author Hank Elsesser
 * @brief A variation of the ECC-RED with fast reductions where degeneracy orderings are used to speed up computation
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
#include <queue>

#include "ecc-nec.h"


class Graph;
class Node;
class Edge;
class Clique;
class Fast_set;

using namespace std;

#ifndef ECC_FR1_H
#define ECC_FR1_H

class ECC_FR1 : public ECC_NEC {
public:
    ECC_FR1() = delete;                 
    ECC_FR1(string dataset_filepath);   // Calls parent constructor + builds lookup set
    ECC_FR1 (Graph& Graph);

    //FROM NEC c_e
        // - Index i corresponds to the number of edges in the neighborhood of edge i
        // vector<size_t> edge_intersection_counts; 

    // New Datastructure: Index i corresponds to all of the common neighbors between edge i
    
    vector<size_t> edge_4_clique_memberships; 
    vector<vector<Node*>> common_neighbor_sets;
    vector<bool> edge_removals;

    virtual vector<Clique*>* run(); 
 
protected:


    //Overrides
    virtual Clique* find_clique_of(Edge* edge);

    virtual size_t apply_rule_one();
    virtual size_t apply_rule_two();
    virtual void get_edge_ordering();


    // virtual Edge* select_uncovered_edge(int& previous_index);
    // virtual void init_edge_intersection_counts();


    // New Methods
    void init_reduction_data(); // For each edge, finds and stores all neighbors of both of its nodes

    virtual vector<Node*> get_degeneracy_ordering(vector<size_t>& degeneracy_indices);
    // virtual vector<Node*> get_degeneracy_ordering();


    void update_4_clique_counts(Edge* edge, vector<Node*>& common_neighbors);

    vector<Node*> get_common_neighbors(Edge*); // Returns the common neighbors of the nodes in an edge

    void remove_node(Node* node);
    void remove_edge(Edge* edge);


    bool edge_removed(Edge* edge);

    
    

};



#endif