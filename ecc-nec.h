/**
 * @file ecc-nec.h
 * @author Hank Elsesser
 * @brief A variation of the ECC-RED with clique formation based on common neighbor edge count "Neighborhood edge counts"
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

#include "ecc-red.h"


class Graph;
class Node;
class Edge;
class Clique;
class Fast_set;

using namespace std;

#ifndef ECC_NEC_H
#define ECC_NEC_H

class ECC_NEC : public ECC_RED {
public:
    ECC_NEC() = delete;                 
    ECC_NEC(string dataset_filepath);   // Calls parent constructor + builds lookup set
    ECC_NEC (Graph& Graph);


    // New Datastructures
    // - Index i corresponds to the number of edges in the neighborhood of edge i
    vector<size_t> edge_intersection_counts;  
    vector<Edge*> edge_ordering; 

    virtual vector<Clique*>* run(); 
 
protected:


    //Overrides
    virtual Edge* select_uncovered_edge(int& previous_index);


    // New Methods
    virtual void init_edge_intersection_counts();
    virtual void get_edge_ordering();


    // size_t apply_rule_two();
    // size_t apply_rule_one();

    // bool is_clique(vector<Node*>&);

    // bool is_removed(Node* node);
    

};



#endif