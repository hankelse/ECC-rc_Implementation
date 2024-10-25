/**
 * @file ecc-fs.h
 * @author Hank Elsesser
 * @brief A variation of the algorithm using fast set to speed up node_set_intersections
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
    ECC_FS() = delete;                 
    ECC_FS(string dataset_filepath);   // Calls parent constructor + builds lookup set

    // New Datastructure
    Fast_set* lookup_set;
 
protected:

    // New Methods
    virtual void trim_candidates(vector<Node *>& candidates, const vector<Node*>& trimming_vec);

    // Methods that are being overridden
    virtual vector<Node *> node_set_intersect(const vector<Node*>&, const vector<Node *>&);
    virtual Clique* find_clique_of(Edge* edge);
    virtual Node* extract_node(vector<Node*>& candidate_set, Clique* clique);

};



#endif