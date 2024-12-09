// Generic common-use functions


#ifndef TOOLS_H
#define TOOLS_H

/* Quickly remove from vector by value*/
template<typename T>
T swap_and_pop(vector<T> vec, T item) {
    auto it = find(vec.begin(), vec.end(), item);
    swap(*it, vec.back());
    return vec.pop_back();
};


#endif

