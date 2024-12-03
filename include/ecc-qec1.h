/**
 * @file ecc-qec1.h
 * @author Hank Elsesser
 * @brief A variation of the algorithm using inline edge covering, and not saving clique edges
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

#include "ecc-fs.h"


class Graph;
class Node;
class Edge;
class Clique;
class Fast_set;

using namespace std;

#ifndef ECC_QEC1_H
#define ECC_QEC1_H

class ECC_QEC1: public ECC_FS {
public:
    ECC_QEC1() = delete;                 
    ECC_QEC1(string dataset_filepath);   // Calls parent constructor + builds lookup set
    ECC_QEC1(Graph& G);

    // New data structure for checking covered edges quickly, initialized in constructor
    vector<int> node_clique_assignments;


protected:

    // New Methods
    virtual void add_to_clique(Clique* clique, Node* node);
    virtual void fill_clique(Clique* clique);

    // // Methods that are being overridden
    // virtual vector<Node *> node_set_intersect(const vector<Node*>&, const vector<Node *>&);
    virtual Clique* find_clique_of(Edge* edge);
    // virtual Node* extract_node(vector<Node*>& candidate_set, Clique* clique);

};



#endif