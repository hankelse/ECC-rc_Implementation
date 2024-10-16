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


class Graph;
class Node;
class Edge;
class Clique;
class Fast_set;

using namespace std;

#ifndef ECC_H
#define ECC_H

class ECC {
public:
    ECC();                 //No default constructor
    ECC(string dataset_filepath);   //Builds ECC object for dataset, and G from dataset
    ECC(Graph& G);            //Builds ECC object for dataset, given G

    vector<Clique*>* run();         //Runs the algorithm Returns a pointer to the clique_cover

    bool check_solution();

    const Graph* graph(); // Returns a pointer to the graph

    string dataset_filepath;
private:
    

protected:
    //Member Variables:
    Graph* G = nullptr; //Constructed with Object
    vector<Clique*> clique_cover;
    size_t num_edges_covered;



    // Framework Methods
    virtual Edge* select_uncovered_edge(int& previous_index);
    virtual Clique* find_clique_of(Edge* edge);
    virtual Node* extract_node(unordered_set<Node*>& candidate_set, Clique* clique);

    //Helper methods
    virtual unordered_set<Node *> node_set_intersect(unordered_set<Node *>&, unordered_set<Node *>&);






};



#endif