#include "checks.h"
#include "io.h"
#include "node.h"
#include "edge.h"

/**
 * @brief Checks if all of the nodes have been imported correctly:
 *        Compares the number of reported nodes to the number of nodes saved in the list
 *        Has suggestions which are shared if there is an error.
 * @param G  The graph
 * @param warnings  The osstringstream to push warnings to
 * @return true  All nodes accounted for!
 * @return false   Something weird happened!
 */
bool all_nodes_imported(Graph& G, ostringstream& warnings) { 
    int reported_num_nodes = G._num_nodes; // The number of nodes the dataset said it would have

    // Check if ._nodes needed more space than was allocated
    if (G._nodes.size() > reported_num_nodes) {
        warnings << "\t NOTE: G._nodes required more space: node ids may not be continuous or start at 0" << endl;
    }

    //Check if there are any blank spaces for nodes
    vector<int> blank_indices; // all the indices in _nodes that are blank
    int num_saved_nodes = 0; // the number of nodes in G
    for (int i = 0; i < G._nodes.size(); i++) {
        if (G._nodes[i] == nullptr) {
            blank_indices.push_back(i);
        } else {
            num_saved_nodes += 1;
        }
    }

    warnings << "\t NOTE: There are " << blank_indices.size() << " blank indices in G._nodes!" << endl;
    if (blank_indices.size() > 0) {
        warnings << "\t\tHere they are: " << blank_indices << endl;
    }

    // Put result message before others
    ostringstream prev_warnings;
    prev_warnings << warnings.str(); //copies warnings to prev
    warnings.str(""); //clears warnings
    warnings << "CHECK FAILURE: all_nodes_imported:  \t Reported number of nodes (" << reported_num_nodes <<") != (" << num_saved_nodes << ") the number of nodes saved." << endl;
    warnings << prev_warnings.str();

    //returns true/false
    return num_saved_nodes == reported_num_nodes;
}


/**
 * @brief Checks if all of the edges have been imported correctly:
 *        Compares number of reported edges to the number of edges saved in the vector
 *        Gives suggestions which are shared if there is an error
 * 
 * @param G 
 * @param warnings 
 * @return true 
 * @return false 
 */
bool all_edges_imported(Graph& G, ostringstream& warnings) {
    int num_edges_reported = G._num_edges;
    int num_empty_indices = 0; // in G._edges
    for (int i = 0; i < G._edges.size(); i++) {
        if (G._edges[i] == nullptr) {
            num_empty_indices += 1;
        }
    }
    warnings << "CHECK FAILURE: all_edges_imported:  \t Reported number of edges (" << num_edges_reported <<") != (" << G._edges.size()-num_empty_indices << ") the number of nodes saved." << endl;
    warnings << "\t NOTE: There are " << num_empty_indices << " empty indices in G._edges." << endl;
    warnings << "\t NOTE: The size of G._edges (" << G._edges.size() << ") had to be changed from the expected size of (" << num_edges_reported << ")." << endl;
    
    return num_edges_reported == (G._edges.size() - num_empty_indices);
}


/**
 * @brief Checks if the adjacency lists of each node match the nodes from iterating through the G._edges vector
 *        Doesn't check all because of very slow running time. Checks 400 times equally spaced if there are more than 400 nodes.
 * @param G
 * @param warnings 
 * @return true 
 * @return false 
 */
bool adj_lists_correct(Graph& G, ostringstream& warnings) {
    int check_limit = 400; //so it doesn't take too long

    bool result = true;
    int num_checks = G._nodes.size();
    if (num_checks > check_limit) {
        num_checks = check_limit;
    }
    // vector<size_t> indicies_to_check = {0, G._nodes.size()/4 , G._nodes.size()/2, 3*G._nodes.size()/4, G._nodes.size()-1};

    warnings << "CHECK FAILURE: adj_lists_correct \t adjacency lists don't match references." << endl;
    for (int i = 0; i < G._nodes.size(); i=i+G._nodes.size()/num_checks) {
        Node* current = G._nodes[i];
        if (current == nullptr) {
            cout << "\t\t\t no node at " << i << endl;
            continue;
        }
 
        if ((i / (G._nodes.size()/num_checks)) % (num_checks/100) == 0) {
            cout <<"\t\t Checked " << (100*i)/(G._nodes.size()) << "% of the lists" << endl;
        }

        //get the neighbor list and edge list to compare against
        vector<Node*> neighbors_reference = current->get_neighbors(G);
        vector<Edge*> edges_reference = current->get_edges(G);

        if (current->neighbors != neighbors_reference) {
            warnings << "\t\t [" << current->id() << "]'s NEIGHBOR adjac list doesn't match reference" << endl;
            warnings << "\t\t\tFrom ->neighbors : " << current->neighbors << endl;
            warnings << "\t\t\tFrom   reference : " << neighbors_reference << endl;
            result = false;
        }
        if (current->edges != edges_reference) {
            warnings << "\t\t [" << current->id() << "]'s EDGES adjac list doesn't match reference" << endl;
            result = false;
        }
    }
    return result;


}

/**
 * @brief Compares vectors as unordered to see if they contain the same things
 * 
 * @tparam S 
 * @param v1 
 * @param v2 
 * @return true 
 * @return false 
 */
template <typename S>
bool vectors_contents_same(vector<S>& v1, vector<S>& v2) {
    if (v1.size() != v2.size()) { return false; }
    
    // vector<S> v1_copy = v1; //make copy so elements can be removed from v1

    // for (int i = 0; i < v1.size()) {

    //     for (int j = 0; j < v2.size()) {

    //     }
    // }

    return true;
    
}
