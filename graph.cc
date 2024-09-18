
#include "graph.h"


Graph::Graph() {
    cout << "WHY DEFAULT CONSTRUCTOR?" << endl;
}

Graph::Graph(string filename) {
    this->construct(filename); //populate the graph from the file.
}

Graph::~Graph() {
    for (int n = 0; n < _num_edges; n++) {
        delete _edges[n];
    }

    for (int i = 0; i < _num_nodes; i++) {
        delete _nodes[i];
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






