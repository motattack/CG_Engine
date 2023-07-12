#ifndef CG_ENGINE_BASESYSTEM_H
#define CG_ENGINE_BASESYSTEM_H

#include <CG_Engine/ecs/base/types.h>

class BaseSystem {
public:
    BaseSystem() = default;

    virtual ~BaseSystem() = default;

    void removeEntity(const EntityId entity) {
        entities.erase(entity);
    }

    void addEntity(const EntityId entity) {
        entities.insert(entity);
    }

    const EntitySignature GetSignature() {
        return signature;
    }

    template<class T>
    void addComponentSighature() {
        signature.insert(CompType<T>());
    }

    virtual void start() {};

    virtual void update() {};

    virtual void destroy() {};
protected:
    friend class EntityManager;

    EntitySignature signature;
    std::set<EntityId> entities;

};

#endif //CG_ENGINE_BASESYSTEM_H
