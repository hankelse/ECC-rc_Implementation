
#ifndef ADJ_LIST_H
#define ADJ_LIST_H

#include <vector>

class Graph;

class Adj_List {
public:
    Adj_List();
    Adj_List(Graph& G);
    ~Adj_List();


private:
    void construct(Graph& G);
    
};



#endif