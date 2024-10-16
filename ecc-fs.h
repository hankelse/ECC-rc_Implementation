/**
 * @file ecc.h
 * @author Hank Elsesser
 * @brief The framework of the ECC algorithm
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
#include <gperftools/profiler.h>

#include "ecc.h"


class Graph;
class Node;
class Edge;
class Clique;
class Fast_set;

using namespace std;

#ifndef ECC_FS_H
#define ECC_FS_H

class ECC_FS : public ECC{
public:
    ECC_FS() = delete;                 //No default constructor
    ECC_FS(string dataset_filepath); // calls parent constructor 

    Fast_set* lookup_set;
protected:
    //new method
    virtual void trim_candidates(vector<Node *>& candidates, const vector<Node*>& trimming_vec);

    // Methods that are being overridden
    virtual vector<Node *> node_set_intersect(const vector<Node*>&, const vector<Node *>&);
    virtual Clique* find_clique_of(Edge* edge);
    virtual Node* extract_node(vector<Node*>& candidate_set, Clique* clique);




};



#endif