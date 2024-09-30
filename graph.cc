
#include "graph.h"
#include "connection.h"
#include "node.h"
#include "edge.h"
#include "io.h"


Graph::Graph() {
    cout << "WHY DEFAULT CONSTRUCTOR?" << endl;
}

Graph::Graph(string filename) {
    this->construct(filename); //populate the graph from the file.
}

Graph::~Graph() {
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
    this->_nodes = vector<Node*>(_num_nodes, nullptr);    // need to be able to set values via indexing in this range
    this->_edges.reserve(this->_num_edges);               // just need to push into this range
    
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
 * WARNING: may break if nodes arent numbered from 0 to num_nodes.
 * WARNING: may break if there are nodes that aren't connected. 
 * 
 * @param file the ifstream after get_sizes has been run.
 */
void Graph::fill_graph(ifstream& file) {
    // get rid of the stuff before the numbers:
    string temp;
    file >> temp; // #
    file >> temp; // From node
    file >> temp; // To node

    // get data
    int node1_id;
    int node2_id;
    while (file >> node1_id) {
        file >> node2_id;

        if (node1_id == node2_id) {
            if (_nodes[node1_id] == nullptr) {
                _nodes[node1_id] = new Node(node1_id);
            }
            _num_edges --; 
            continue;
        }

        //If more space is needed for nodes, resize: (Good indicator of error, caught by checksß)
        if (node1_id >= _nodes.size()) {
            _nodes.resize(node1_id+1);
        }
        if (node2_id >= _nodes.size()) {
            _nodes.resize(node2_id +1);
        }

        //If nodes haven't been created, create them
        if (_nodes[node1_id] == nullptr) {
            _nodes[node1_id] = new Node(node1_id);
        }
        if (_nodes[node2_id] == nullptr) {
            _nodes[node2_id] = new Node(node2_id);
        }
        
        // Add the edge
        Edge* temp_edge;
        temp_edge = new Edge(_nodes[node1_id], _nodes[node2_id]);
        _edges.push_back(temp_edge);

        //Add the each node to the other adjacency neighbors list
        _nodes[node1_id]->neighbors.push_back(_nodes[node2_id]);
        _nodes[node2_id]->neighbors.push_back(_nodes[node1_id]);

        //Add the edge to each nodes adjacency edges list
        _nodes[node1_id]->edges.push_back(temp_edge);
        _nodes[node2_id]->edges.push_back(temp_edge);

        //Add each to each nodes adjacency Connection list
        _nodes[node1_id]->connections.push_back(new Connection(temp_edge, _nodes[node2_id]));
        _nodes[node2_id]->connections.push_back(new Connection(temp_edge, _nodes[node1_id]));
    }

}

/**
 * @brief selects an uncovered edge in G starting where it left off.
 * 
 * @param previous_index 
 * @return Edge* 
 */
Edge* Graph::select_uncovered_edge(int& previous_index) {
    for (int i = previous_index; i < _num_edges; i++) {
        if (!_edges[i]->is_covered()) {
            previous_index = i;
            return _edges[previous_index];
        } else {
            cout << "\t" << _edges[i] << "is covered?" << _edges[i]->is_covered() << endl;
        }
    }
    cerr << "Got to end of select_uncovered_edge!" << endl;
    return nullptr;
}


/**
 * @brief Find the clique of an edge
 * 
 * @param edge_uv 
 * @return vector<Edge*> 
 */
vector<Edge*> Graph::find_clique_of(Edge* edge_uv) {
    //R ← {u, v}
    vector<Edge*> result = {edge_uv};

    //P ← N(u)∩ N(v)
    vector<Node*> potential_additions;
    Node* u = edge_uv->_node1;
    Node* v = edge_uv->_node2;

    // LEFT ON: WHAT SHOULD BE A CONNECTION AND WHAT SHOULD BE AN EDGE?
    potential_additions = node_set_intersect(u->neighbors, v->neighbors);

    // z ← EXTRACT NODE(P)

    //get all the nodes in result
    vector<Node*> result_nodes;

    Node* new_addition = extract_node(potential_additions, result_nodes);
    
    return result;
}

/**
 * @brief Return any vertex z (if any) yielding maximum size |N_u(z) ∩ R| > 0; otherwise, return z = null.
 * 
 * @param potential_additions P - the set of nodes that could be in the clique
 * @param clique R - the set of nodes in the clique
 * @return Node* z // nullptr if there is none
 */
Node* Graph::extract_node(vector<Node*> potential_additions, vector<Node*>& clique) {
    Node* best = nullptr;
    int best_score = 0;

    //Find the z who has the most uncovered neighbors that are in R ()
    for (int i = 0; i < potential_additions.size(); i++) {
        Node* z = potential_additions[i];
        int z_score = 0;       //the number of uncovered edges between z and R
        
        // Iterate through R, looking for uncovered edges between the nodes in R and z
        for (int i = 0; i < clique.size(); i++) {
            Edge* connection = this->are_connected(z, clique[i]); //nullptr if none found
            if (connection != nullptr && !connection->is_covered()) {
                z_score += 1;
            }
        }

        //update best and best_score if better z is found
        if (z_score > best_score) {
            best_score = z_score;
            best = z;
        }
 
    }
    return best; //nullptr if none better than 0
}

/**
 * @brief Slow program give N(u) intersect N(v) where u and v are nodes
 * 
 * @param u_neighbors 
 * @param v_neighbors 
 * @return vector<Node*> 
 */
vector<Node*> node_set_intersect(vector<Node*> u_neighbors, vector<Node*> v_neighbors) {
    vector<Node*> result = u_neighbors;
    
    bool unique_to_v;
    for (int i = 0; i < v_neighbors.size(); i++) {
        Node* v_neighbor = v_neighbors[i];
        unique_to_v = true;
        for (int _ = 0; _ < result.size(); _++) {
            if (v_neighbor->id() == result[_]->id()) {
                unique_to_v = false;
                continue;
            }
        }
        if (unique_to_v) {
            result.push_back(v_neighbor);
        }
    }
    return result;
}

/**
 * @brief Checks if two nodes are connected and returns the connecting edge if they are
 * 
 * @param node1 
 * @param node2 
 * @return Edge* 
 */
Edge* Graph::are_connected(Node* node1, Node* node2) {
    vector<Connection*> n1_connects = node1->connections;
    for (int i = 0; i < n1_connects.size(); i++) {
        Connection* connection = n1_connects[i];
        if (*connection->neighbor == *node2) {
            return connection->edge;
        }
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
Edge* Graph::get_edge(Node* node1, Node* node2) {
    vector<Connection*> n1_connects = node1->connections;
    for (int i = 0; i < n1_connects.size(); i++) {
        Connection* connection = n1_connects[i];
        if (connection->neighbor->id() == node2->id()) {
            return connection->edge;
        }
    }

    return nullptr;
}

/**
 * @brief NOT IMPLEMENTED- Given a vector of edges get the nodes in the list
 * Used for the confusion between functions that need nodes and functions that need edges
 * 
 * @param edges 
 * @return vector<Node*> 
 */
vector<Node*> Graph::edges_to_nodes(vector<Edge*> edges) {
    vector<Node*> result;
    for (int i = 0; i < edges.size(); i++) {
        Edge* edge = edges[i];

        bool node1_already_added = false;
        bool node2_already_added = false;

        for (int j = 0; j < result.size(); j++) {
            if (*result[j] == *edge->_node1) {
                node1_already_added = true;
            } else if (*result[j] == *edge->_node2) {
                node2_already_added = true;
            }
            if (!node1_already_added) {
                result.push_back(edge->_node1);
            }
            if (!node2_already_added) {
                result.push_back(edge->_node2);
            }

        }

    }
    return {nullptr};
}

/**
 * @brief NOT IMPLEMENTED- Given a vector of nodes get the all edges in the list
 * Used for the confusion between functions that need nodes and functions that need edges
 * 
 * @param nodes 
 * @return vector<Edge*> 
 */
vector<Edge*> Graph::nodes_to_edges(vector<Node*> nodes) {
    return {nullptr};
}