

#ifndef NODE_H
#define NODE_H

#include <iostream>
using namespace std;

class Node {
    public:
        Node(int id);
        Node();
        

        int id();

        

    private:
        int _id;

};
    
ostream& operator<<(ostream& os, Node& node);

#endif