#ifndef CG_ENGINE_BASECOMPONENT_H
#define CG_ENGINE_BASECOMPONENT_H

#include <CG_Engine/ecs/base/types.h>

class BaseComponent {
private:
    friend class EntityManager;

    EntityId entityId;
public:
    BaseComponent() : entityId() {};

    virtual ~BaseComponent() {};

    const EntityId GetId() {
        return entityId;
    }
};

#endif //CG_ENGINE_BASECOMPONENT_H
