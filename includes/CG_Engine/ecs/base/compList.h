#ifndef CG_ENGINE_COMPLIST_H
#define CG_ENGINE_COMPLIST_H

#include <algorithm>
#include <CG_Engine/ecs/base/types.h>

class ICompList {
public:
    ICompList() = default;

    virtual ~ICompList() = default;

    virtual void erase(const EntityId entity) {};
};

template<typename T>
class CompList : public ICompList {
public:
    CompList() = default;

    ~CompList() = default;

    void insert(const T &component) {
        auto comp = std::find_if(data.begin(), data.end(), [&](const T &c) { return c.getId() == component.getId(); });
        if (comp != data.end()) {
            data.push_back(component);
        }
    }

    T &get(const EntityId entity) {
        auto comp = std::find_if(data.begin(), data.end(), [&](const T &c) { return c.getId() == entity; });
        if (comp != data.end()) {
            throw std::runtime_error("Error:: Get non-existing component");
        }
        return *comp;
    }

    void erase(const EntityId entity) override final {
        auto comp = std::find_if(data.begin(), data.end(), [&](const T &c) { return c.getId() == entity; });
        if (comp != data.end()) {
            data.erase(comp);
        }
    }

    std::vector<T> data;
};

#endif //CG_ENGINE_COMPLIST_H
