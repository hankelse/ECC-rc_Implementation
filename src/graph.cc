
#include "graph.h"
#include "connection.h"
#include "node.h"
#include "edge.h"
#include "io.h"
#include "clique.h"
#include <unordered_set>
#include <unordered_map>


Graph::Graph() {
    cout << "WHY DEFAULT CONSTRUCTOR?" << endl;
}

Graph::Graph(string filename) {
    this->filename = filename;
    this->construct(filename); //populate the graph from the file.
    // cout << "\t graph constructed: " << filename << endl;
}

Graph::~Graph() {
    // cout << "\t deconstructing G : " << filename << endl;
    for (int n = 0; n < _edges.size(); n++) {
        // if (_edges[n] != nullptr) {
        delete _edges[n];
        // }
    }

    for (int i = 0; i < _nodes.size(); i++) {
        Node* node = _nodes[i];
        if (_nodes[i] == nullptr) {
            continue;
        }
        for (int n = 0; n < node->connections.size(); n++) {
            delete node->connections[n];
        }

        delete node;
    }

    for (int i = 0; i < _cliques.size(); i ++) {
        delete _cliques[i];
    }


    return;
}


/**
 * @brief Parent function for filling the graph with nodes and edges.
 * Calls get_data_sizes and fill_graph
 * 
 * @param filename of the dataset
 */
void Graph::construct(string filename) {
    ifstream file(filename);

    //Check if file was opened correctly
    if (!file.is_open()) {
        cerr << "Error: Could not open the file " << filename << endl;
    }

    //Get the reported number of nodes and edges in G
    this->get_data_sizes(file);

    // reserve neccesary space with the new sizes
    // this->_nodes = vector<Node*>(_num_nodes, nullptr);  
    // this->_nodes  
    this->_edges.reserve(this->_num_edges);               
    
    //read in the data: push edge*s, assign node* to id spot
    this->fill_graph(file);

    file.close();
}   


/**
 * @brief From the heading of the dataset, finds the number of nodes and edges in the data.
 * 
 * @param file the ifstream from the file to read from it.
 */
void Graph::get_data_sizes(ifstream& file) {
    bool sizes_found = false; // have we found "Nodes:"?
    
    int num_nodes = 0;
    int num_edges = 0;

    string word; // holds current word/number from file
    while (!sizes_found) {
        file >> word;
        if (word == "Nodes:") {
            sizes_found = true;
            file >> num_nodes;
            file >> word;
            file >> num_edges;
        }
    }
    this->_num_nodes = num_nodes;
    this->_num_edges = num_edges;
}


/**
 * @brief Gets rid of remaining unnessesary data and 
 * reads in the edges and nodes into _nodes and _edges.
 * 
 * Does not include any self connections
 * 
 * 
 * @param file the ifstream after get_sizes has been run.
 */
void Graph::fill_graph(ifstream& file) {
    // get rid of the stuff before the numbers:
    string temp;
    file >> temp; // #
    file >> temp; // From node
    file >> temp; // To node

    unordered_map<int, int> id_to_index; 

    // get data
    int node1_id;
    int node2_id;
    Node* node1;
    Node* node2;


    while (file >> node1_id) {
        file >> node2_id;

        // If there is a self connecting edge
        if (node1_id == node2_id) {
            if (id_to_index.find(node1_id) == id_to_index.end()) {
                id_to_index[node1_id] = _nodes.size();
                node1 = new Node(node1_id, _nodes.size());
                _nodes.push_back(node1);
            }
            _num_edges --; 
            continue;
        }

        //If nodes haven't been created, create them
        if (id_to_index.find(node1_id) == id_to_index.end()) {
                id_to_index[node1_id] = _nodes.size();
                node1 = new Node(node1_id, _nodes.size());
                _nodes.push_back(node1);
        } else {
            node1 = _nodes[id_to_index[node1_id]];
        }
        if (id_to_index.find(node2_id) == id_to_index.end()) {
                id_to_index[node2_id] = _nodes.size();
                node2 = new Node(node2_id, _nodes.size());
                _nodes.push_back(node2);
        } else {
            node2 = _nodes[id_to_index[node2_id]];
        }
        
        // Add the edge
        Edge* edge;
        //if reversed edge hasn't been added already
        bool new_edge = true;
        for (Node* neighbor : node2->neighbors) {
            if (neighbor->id() == node1_id) {
                new_edge = false;
                continue;
            }
        }
        if (new_edge) {
            edge = new Edge(node1, node2);
            edge->index = _edges.size(); // save index for Clique::add_node_cf for ECC-CF
            _edges.push_back(edge);
            
        } else {
            _num_edges -= 1; //decrease num reported edges by 1
        }
        if (new_edge) {

        //Add the each node to the other adjacency neighbors list
        node1->neighbors.push_back(node2);
        node2->neighbors.push_back(node1);

        //Add the edge to each nodes adjacency edges list
        node1->edges.push_back(edge);
        node2->edges.push_back(edge);

        //Add each to each nodes adjacency Connection list
        node1->connections.push_back(new Connection(edge, node2));
        node2->connections.push_back(new Connection(edge, node1));

        //Add each to each nodes connection map
        node1->connection_map[node2] = edge;
        node2->connection_map[node1] = edge;

        //Add each node to eachother's neighbor set
        node1->neighbor_set.insert(node2);
        node2->neighbor_set.insert(node1);

        }

        


    }

}

/**
 * @brief FASTER! Using connection_map (unordered_map): Checks if two nodes are connected and returns the connecting edge if they are
 * 
 * @param node1 
 * @param node2 
 * @return Edge* 
 */
Edge* Graph::are_connected(Node* node1, Node* node2) const {
    if (node1 == nullptr | node2 == nullptr) {
        cerr << "PASSED NULLPTR TO are_connected" << endl;
        return nullptr;
    }

    unordered_map<Node*, Edge*>::const_iterator node2_searcher = node1->connection_map.find(node2);
    if (node2_searcher != node1->connection_map.end()) {
        return node2_searcher->second;
    }

    return nullptr;
}


/**
 * @brief Synonym of are_connected
 * 
 * @param node1 
 * @param node2 
 * @return Edge* 
 */
Edge* Graph::get_edge(Node* node1, Node* node2) const  {
    return are_connected(node1, node2);
}




// /**
//  * @brief Creates a Clique object representing the clique with the edge
//  * 
//  * @param edge_uv 
//  * @return vector<Edge*> 
//  */
// Clique* Graph::find_clique_of(Edge* edge, size_t& edges_covered) {

//     //R ← {u, v}  create the new clique with the edge
//     Clique* clique = new Clique(edge, *this, edges_covered);
//     Node* u = edge->_node1;
//     Node* v = edge->_node2;

//     //save the clique (for deconstruction)
//     _cliques.push_back(clique); 

//     //P ← N(u)∩ N(v)
//     unordered_set<Node*> candidates = node_set_intersect(u->neighbor_set, v->neighbor_set);

//     // vector<Node*> candidates = node_set_intersect(u->neighbors, v->neighbors);

//     // vector<Node*> candidates_vector = node_vector_intersect(u->neighbors, v->neighbors);
//     // unordered_set<Node*> candidates(candidates_vector.begin(), candidates_vector.end());

//     // z ← EXTRACT NODE(P)
//     // Node* new_member = extract_node(candidates, clique);
//     Node* new_member = extract_node(candidates, clique);

//     // while there are more potential additions:
//     while (new_member != nullptr) {
//         //add new_node to clique (COVERS EDGES)
//         clique->add_node(new_member, *this, edges_covered);

//         // Trim candidates: P ← P ∩ N(z)
//         // candidates = node_set_intersect(candidates, new_member->neighbors);
//         // candidates = trim_set(candidates, new_member->neighbors);
//         // trim_candidates(candidates, new_member->neighbor_set);
//         candidates = node_set_intersect(candidates, new_member->neighbor_set);

//         // Extract next node
//         // new_member = extract_node(candidates, clique);
//         new_member = extract_node(candidates, clique);

//     }
//     return clique;
// }

// /**
//  * @brief Return any vertex z (if any) yielding maximum size |N_u(z) ∩ R| > 0; otherwise, return z = null.
//  * 
//  * @param potential_additions P - the set of nodes that could be in the clique
//  * @param clique R - The clique being built
//  * @return Node* z // nullptr if there is none
//  */
// Node* Graph::extract_node(unordered_set<Node*>& potential_additions, Clique* clique) {
//     //If no potential_additions
//     if (potential_additions.empty()) {
//         return nullptr;
//     }

//     Node* best = nullptr;
//     int best_score = 0;

//     /* ... process *itr ... */

//     //Find the z who has the most uncovered neighbors that are in R ()
//     for (unordered_set<Node*>::const_iterator itr = potential_additions.begin(); itr != potential_additions.end(); ++itr) {
//         Node* z = *itr;
//         int z_score = 0;       //the number of uncovered edges between z and R
        
//         // Iterate through R, looking for uncovered edges between the nodes in R and z
//         for (int j = 0; j < clique->nodes.size(); j++) {
//             Edge* connection = this->are_connected(z, clique->nodes[j]); //nullptr if none found
//             if (connection != nullptr && !connection->is_covered()) {
//                 z_score += 1;
//             }
//         }

//         //update best and best_score if better z is found
//         if (z_score >= best_score) {
//             best_score = z_score;
//             best = z;
//         }
 
//     }
//     return best;
// }

// Node* Graph::extract_node(unordered_set<Node*>& potential_additions, Clique* clique) {
//     //If no potential_additions
//     if (potential_additions.empty()) {
//         return nullptr;
//     }

//     // Create a vector to track the number of uncovered connections between each potential node and the clique nodes
//     unordered_map<Node*, int> uncovered_counts;

//     // Iterate over the clique's nodes and check for uncovered edges
//     for (Node* clique_node : clique->nodes) {
//         for (Node* potential : potential_additions) {
//             // Use are_connected() to check if the node is connected to the clique node and if the edge is uncovered
//             Edge* connection = this->are_connected(potential, clique_node);
//             if (connection != nullptr && !connection->is_covered()) {
//                 uncovered_counts[potential] += 1;
//             }
//         }
//     }

//     // Find the node with the maximum uncovered connections
//     Node* best_node = nullptr;
//     int best_score = 0;
    
//     for (const auto& [node, score] : uncovered_counts) {
//         if (score > best_score) {
//             best_node = node;
//             best_score = score;
//         }
//     }

//     return best_node;
// }



// /**
//  * @brief  Even Faster! Given an unordered_set and vector, returns the elements in both
//  * @param set_1
//  * @param vec_2
//  * @return unordered_set<Node *> new set
//  */
// unordered_set<Node *> Graph::trim_set(unordered_set<Node *> set_1, vector<Node*>& vec_2) {
//     unordered_set<Node*> result;
//     for (Node* node : vec_2) {
//         if (set_1.count(node) > 0) {
//             result.insert(node);
//         }
//     }
//     return result;
// }

// void Graph::trim_candidates(unordered_set<Node *>& candidates, unordered_set<Node *>& constraints){
//     if (candidates.size() >= constraints.size()) {
//         candidates = node_set_intersect(constraints, candidates);
//         return;
//     }

//     // Iterate over the candidates and remove any that are not in the constraints
//     for (unordered_set<Node*>::const_iterator it = candidates.begin(); it != candidates.end(); ) {
//         // If the candidate is not in constraints, erase it from candidates
//         if (constraints.find(*it) == constraints.end()) {
//             it = candidates.erase(it);  // Erase returns the next iterator
//         } else {
//             ++it;  // Move to the next element if not erased
//         }
//     }
// }

 
// /**
//  * @brief  Faster! Given two sets of nodes, returns the elements in both
//  * @param vec_1
//  * @param vec_2
//  * @return vector<Node*> 
//  */
// vector<Node*> Graph::node_vector_intersect(vector<Node*>& vec_1, vector<Node*>& vec_2) {

//     unordered_set<Node *> set_1(vec_1.begin(), vec_1.end()); //O(n)
//     vector<Node*> intersection;

//     for (Node* node : vec_2) {
//         if (set_1.count(node) > 0) {
//             intersection.push_back(node);
//         }
//     }

//     return intersection;
// }

// unordered_set<Node *> Graph::node_set_intersect(unordered_set<Node *>& set_1, unordered_set<Node *>& set_2) {
//     // for (Node* node : set_2) {
//     //     if (set_1.count(node) == 0) {
//     //         set_2.erase(node);
//     //     }
//     // }
//     int correct_index = 0;
//     for (unordered_set<Node*>::iterator itr = set_1.begin(); itr != set_2.end(); ++itr) {
//         if (set_2.count(*itr) == 0) {
//             set_1.erase(*itr);
//             correct_index -= 1;
//             --itr;
//         } 
//         correct_index += 1;
//     }
//     return set_2;
// }


// unordered_set<Node *> Graph::node_set_intersect(unordered_set<Node *>& set_1, unordered_set<Node *>& set_2) {
//     unordered_set<Node*> result;
//     for (Node* node : set_2) {
//         if (set_1.count(node) > 0) {
//             result.insert(node);
//         }
//     }
//     return result;
// }

// unordered_set<Node *> Graph::node_set_intersect(unordered_set<Node *>& set_1, unordered_set<Node *>& set_2) {

//     //Recursively run with smaller first
//     if (set_1.size() > set_2.size()) {
//         return node_set_intersect(set_2, set_1);
//     }

//     unordered_set<Node*> result;
    
//     // Iterate over the smaller set and check for each node in larger
//     for (Node* node : set_1) {
//         if (set_2.count(node) > 0) {
//             result.insert(node);
//         }
//     }
//     return result;
// }



// /**
//  * @brief NOT IMPLEMENTED- Given a vector of edges get the nodes in the list
//  * Used for the confusion between functions that need nodes and functions that need edges
//  * 
//  * @param edges 
//  * @return vector<Node*> 
//  */
// vector<Node*> Graph::edges_to_nodes(vector<Edge*> edges) {
//     vector<Node*> result;
//     for (int i = 0; i < edges.size(); i++) {
//         Edge* edge = edges[i];

//         bool node1_already_added = false;
//         bool node2_already_added = false;

//         for (int j = 0; j < result.size(); j++) {
//             if (*result[j] == *edge->_node1) {
//                 node1_already_added = true;
//             } else if (*result[j] == *edge->_node2) {
//                 node2_already_added = true;
//             }
//             if (!node1_already_added) {
//                 result.push_back(edge->_node1);
//             }
//             if (!node2_already_added) {
//                 result.push_back(edge->_node2);
//             }

//         }

//     }
//     return {nullptr};
// }

// /**
//  * @brief NOT IMPLEMENTED- Given a vector of nodes get the all edges in the list
//  * Used for the confusion between functions that need nodes and functions that need edges
//  * 
//  * @param nodes 
//  * @return vector<Edge*> 
//  */
// vector<Edge*> Graph::nodes_to_edges(vector<Node*> nodes) {
//     return {nullptr};
// }