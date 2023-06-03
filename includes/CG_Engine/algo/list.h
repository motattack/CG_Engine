#ifndef CG_ENGINE_LIST_H
#define CG_ENGINE_LIST_H
// T
#include <vector>
#include <algorithm>

namespace List {
    // find index of item in vector (list)
    template<typename T>
    int getIndexOf(std::vector<T> v, T x) {
        return std::find(v.begin(), v.end(), x) - v.begin();
    }

    // test if list contains item
    template<typename T>
    bool contains(std::vector<T> v, T x) {
        return std::find(v.begin(), v.end(), x) != v.end();
    }
}

#endif //CG_ENGINE_LIST_H
