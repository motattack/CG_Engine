#ifndef CG_ENGINE_COMPLIST_H
#define CG_ENGINE_COMPLIST_H

#include <algorithm>
#include <CG_Engine/base/types.h>

class ICompList {
public:
    ICompList() = default;

    virtual ~ICompList() = default;

    virtual void insert(BaseComponent *component) = 0;

    virtual void erase(EntityId entity) = 0;

    ComponentTypeId getDataType() const { return typeID; }

protected:
    ComponentTypeId typeID = INVALID_TYPE_ID;
};

template<typename T>
class CompList : public ICompList {
public:
    ~CompList() = default;

    CompList() : data({}) {
        typeID = CompType<T>();
    }

    void insert(BaseComponent *component) override {
        T comp = *(static_cast<T *>(component));
        auto it = std::find_if(data.begin(), data.end(), [&comp](const T &c) { return c.getId() == comp.getId(); });
        if (it != data.end())
            std::cout << "Trying to insert duplicate of component!" << std::endl;
        data.push_back(comp);
    }

    T &get(const EntityId entity) {
        auto it = std::find_if(data.begin(), data.end(), [&](const T &comp) { return comp.getId() == entity; });
        if (it == data.end())
            std::cout << "Trying to get non-existing component!" << std::endl;
        return *it;
    }

    void erase(const EntityId entity) override {
        auto it = std::find_if(data.begin(), data.end(), [&entity](const T &comp) { return comp.getId() == entity; });
        if (it != data.end()) {
            data.erase(it);
        }
    }

private:
    std::vector<T> data;
};

#endif //CG_ENGINE_COMPLIST_H
