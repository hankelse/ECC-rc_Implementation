/**
 * @file ecc-fsr.h
 * @author Hank Elsesser
 * @brief A variation of the fast set with data reductions
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

#include "ecc-qec1.h"


class Graph;
class Node;
class Edge;
class Clique;
class Fast_set;

using namespace std;

#ifndef ECC_RED_H
#define ECC_RED_H

class ECC_RED : public ECC_QEC1 {
public:
    ECC_RED() = delete;                 
    ECC_RED(string dataset_filepath);   // Calls parent constructor + builds lookup set
    ECC_RED (Graph& Graph);


    // New Datastructures
    vector<bool> node_removals;
    vector<bool> edge_removals;

    size_t nodes_removed = 0;
    size_t edges_removed = 0;


    virtual vector<Clique*>* run();  // runs reductions then calls ECC's run
 
protected:


    //Overrides
    virtual vector<Node *> node_set_intersect(const vector<Node *>& vec_1, const vector<Node*>& vec_2);
    virtual void trim_candidates(std::vector<Node*>& candidates, const std::vector<Node*>& trimming_vec);
    virtual Node* extract_node(vector<Node*>& potential_additions, Clique* clique);
    virtual Clique* find_clique_of(Edge* edge);


    // New Methods
    virtual void apply_rules_exhaustively();
    virtual size_t apply_rule_two();
    virtual size_t apply_rule_one();

    virtual bool is_clique(vector<Node*>&);

    virtual bool is_removed(Node* node);
    

};



#endif