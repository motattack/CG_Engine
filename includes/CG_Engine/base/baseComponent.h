#ifndef CG_ENGINE_BASECOMPONENT_H
#define CG_ENGINE_BASECOMPONENT_H

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
