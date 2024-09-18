#include <iostream>
using namespace std;
#include <type_traits>

#ifndef IO_H
#define IO_H

/**
 * @brief Overloading << for all types of vectors. 
 * If the type is a pointer, the dereferenced value is printed.
 * 
 * @tparam T  the datatype in the vector
 * @param os  the output stream
 * @param v  the vector to print
 * @return ostream& 
 */
template <typename T>
ostream& operator<<(ostream &os, vector<T> &v) {
    // Check if vector is empty
    if (v.size() < 1) {
        os << "[vector is empty]" << endl;
    } else {

        //Check if T is a pointer
        if constexpr (is_pointer<T>::value) {

            //send DEREFERENCED entries to os
            os << "[" << *v[0]; //no ' ,' before first
            for (int i = 1; i < v.size(); i++) {
                os << ", " << *v[i];
            }
            os << "]";

        } else {

        // send entries to os AS IS
            os << "[" << v[0]; //no ' ,' before first
            for (int i = 1; i < v.size(); i++) {
                os << ", " << v[i];
            }
            os << "]";

        }
    }
    return os;
}

#endif