#ifndef CG_ENGINE_ENTITY_H
#define CG_ENGINE_ENTITY_H

#include <CG_Engine/base/entityManager.h>

class Entity {
private:
    EntityId ID;
public:
    Entity() {
        ID = Manager.addNewEntity();
    }

    EntityId getId() const {
        return ID;
    }

    template<typename T, typename... Args>
    void addComponent(Args &&... args) {
        Manager.addComponent<T>(ID, std::forward<Args>(args)...);
    }

    template<typename T>
    void addComponent(T &component) {
        Manager.addComponent<T>(ID, component);
    }

    template<typename T>
    T &getComponent() {
        return Manager.getComponent<T>(ID);
    }

    template<typename T>
    void removeComponent() {
        Manager.removeComponent<T>(ID);
    }

    template<typename T>
    inline bool hasComponent() {
        return Manager.hasComponent<T>(ID);
    }

    void destroy() {
        Manager.destroyEntity(ID);
    }
};

#endif //CG_ENGINE_ENTITY_H
