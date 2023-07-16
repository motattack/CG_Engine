#ifndef CG_ENGINE_BASECOMPONENT_H
#define CG_ENGINE_BASECOMPONENT_H

#include <CG_Engine/base/types.h>

class BaseComponent {
private:
    EntityId entityID = -1;

    friend class EntityManager;

public:
    BaseComponent() = default;

    ~BaseComponent() = default;

    EntityId getId() const {
        return entityID;
    }
};

#endif //CG_ENGINE_BASECOMPONENT_H
