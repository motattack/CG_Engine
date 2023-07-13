#ifndef CG_ENGINE_ENTITY_H
#define CG_ENGINE_ENTITY_H

#include <CG_Engine/ecs/base/entityManager.h>

class Entity {
private:
    EntityId ID;
    EntityManager *manager;
public:
    Entity(const EntityId id, EntityManager *manager) : ID(id), manager(manager) {};

    ~Entity() = default;

    EntityId getId() {
        return ID;
    }

    template<typename T, typename... Args>
    void addComponent(Args &&... args) {
        manager->addComponent<T>(ID, std::forward<Args>(args)...);
    }

    template<typename T>
    void addComponent(T &component) {
        manager->addComponent<T>(ID, component);
    }

    template<typename T>
    T &getComponent() {
        return manager->getComponent<T>(ID);
    }

    template<typename T>
    void removeComponent() {
        manager->removeComponent<T>(ID);
    }

    template<typename T>
    bool hasComponent() {
        return manager->hasComponent<T>(ID);
    }

    void destroy() {
        manager->destroyEntity(ID);
    }
};

#endif //CG_ENGINE_ENTITY_H
