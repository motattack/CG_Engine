#ifndef CG_ENGINE_BASESYSTEM_H
#define CG_ENGINE_BASESYSTEM_H

class BaseSystem {
protected:
    Signature signature;
    std::set<EntityId> entities;
public:
    BaseSystem() = default;

    virtual ~BaseSystem() = default;

    const Signature getSignature() const { return signature; }

    void eraseEntity(const EntityId entity) { entities.erase(entity); }

    void pushEntity(const EntityId entity) { entities.insert(entity); }

    template<typename T>
    void addComponentSignature() { signature.insert(CompType<T>()); }

    const bool isEmpty() const { return (entities.size() == 0); }

    const bool hasEntity(EntityId entity) const {
        return (entities.count(entity) > 0);
    }

    virtual void stop() {}

    virtual void start() {}

    virtual void render() {}

    virtual void update() {}
};

#endif //CG_ENGINE_BASESYSTEM_H
