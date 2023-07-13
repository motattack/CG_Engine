#ifndef CG_ENGINE_BASESYSTEM_H
#define CG_ENGINE_BASESYSTEM_H

#include <CG_Engine/base/types.h>

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

    Signature getSignature() {
        return signature;
    }

    template<typename T>
    void addComponentSignature() {
        signature.insert(CompType<T>());
    }

    virtual void start() {};

    virtual void update() {
        for (auto i: entities) {
            std::cout << i << "     ";
        }
        std::cout << std::endl;
    };

    virtual void render() {};

    virtual void destroy() {};
protected:
    friend class EntityManager;

    Signature signature;
    std::set<EntityId> entities;

};

#endif //CG_ENGINE_BASESYSTEM_H
