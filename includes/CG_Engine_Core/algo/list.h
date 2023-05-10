#ifndef CG_ENGINE_LIST_H
#define CG_ENGINE_LIST_H

#include <vector>
#include <algorithm>

namespace List {
    template<typename T>
    int getIndexOf(std::vector<T> v, T x) {
        return std::find(v.begin(), v.end(), x) - v.begin();
    }

    template<typename T>
    bool contains(std::vector<T> v, T x) {
        return std::find(v.begin(), v.end(), x) != v.end();
    }
}

#endif //CG_ENGINE_LIST_H
