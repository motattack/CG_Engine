#ifndef CG_ENGINE_ENTITYMANAGER_H
#define CG_ENGINE_ENTITYMANAGER_H

#include <CG_Engine/base/baseComponent.h>
#include <CG_Engine/base/baseSystem.h>
#include <CG_Engine/base/compList.h>
#include <map>
#include <queue>
#include <memory>

class EntityManager {
private:
    EntityId entityCount;
    std::queue<EntityId> availableEntities;
    std::map<EntityId, std::shared_ptr<Signature>> entitiesSignatures;
    std::map<SystemTypeId, std::shared_ptr<BaseSystem>> registeredSystems;
    std::map<ComponentTypeId, std::shared_ptr<ICompList>> componentsArrays;

public:
    EntityManager() : entityCount(0) {
        for (EntityId entity = 0; entity < MAX_ENTITY_COUNT; entity++) {
            availableEntities.push(entity);
        }
    }

    ~EntityManager() {

    }

    void update() {
        for (auto &system: registeredSystems) {
            system.second->update();
        }
    }

    void render() {
        for (auto &system: registeredSystems) {
            system.second->render();
        }
    }

    EntityId addNewEntity() {
        const EntityId entity = availableEntities.front();
        addEntitySignature(entity);
        availableEntities.pop();
        entityCount++;
        return entity;
    }

    void destroyEntity(const EntityId entity) {
        if (entity >= MAX_ENTITY_COUNT) {
            throw std::logic_error("Error: Id is out of range!");
        }
        entitiesSignatures.erase(entity);

        for (auto &array: componentsArrays) {
            array.second->erase(entity);
        }

        for (auto &system: registeredSystems) {
            system.second->removeEntity(entity);
        }
        entityCount--;
        availableEntities.push(entity);
    }

    template<typename T, typename... Args>
    void addComponent(const EntityId entity, Args &&... args) {
        if (entity >= MAX_ENTITY_COUNT) {
            throw std::logic_error("Error:: EntityId is out of range");
        }
        if (entitiesSignatures[entity]->size() >= MAX_COMPONENT_COUNT) {
            throw std::logic_error("Component count limit");
        }

        T component(std::forward<Args>(args)...);
        component.entityId = entity;
        getEntitySignature(entity)->insert(CompType<T>());
        getCompList<T>()->insert(component);
        updateEntityTargetSystems(entity);
    }

    template<typename T, typename... Args>
    void addComponent(const EntityId entity, T &component) {
        if (entity >= MAX_ENTITY_COUNT) {
            throw std::logic_error("Error:: EntityId is out of range");
        }
        if (entitiesSignatures[entity]->size() >= MAX_COMPONENT_COUNT) {
            throw std::logic_error("Component count limit");
        }

        component.entityId = entity;
        getEntitySignature(entity)->insert(CompType<T>());
        getCompList<T>()->insert(component);
        updateEntityTargetSystems(entity);
    }

    template<typename T>
    void removeComponent(const EntityId entity) {
        if (entity >= MAX_ENTITY_COUNT) {
            throw std::logic_error("Error:: EntityId is out of range");
        }

        const ComponentTypeId compType = CompType<T>();
        entitiesSignatures.at(entity)->erase(compType);
        getCompList<T>()->insert(entity);
        updateEntityTargetSystems(entity);
    }

    template<typename T>
    T &getComponent(const EntityId entity) {
        if (entity >= MAX_ENTITY_COUNT) {
            throw std::logic_error("Error:: EntityId is out of range");
        }
        return getCompList<T>()->get(entity);
    }

    template<typename T>
    bool hasComponent(const EntityId entity) {
        if (entity >= MAX_ENTITY_COUNT) {
            throw std::logic_error("Error:: EntityId is out of range");
        }

        return (getEntitySignature(entity)->count(CompType<T>()) > 0);
    }

    template<typename T>
    void registerSystem() {
        const SystemTypeId systemType = SystemType<T>();
        if (registeredSystems.count(systemType) != 0) {
            throw std::runtime_error("System already register");
        }
        auto system = std::make_shared<T>();

        for (EntityId entity = 0; entity < entityCount; entity++) {
            addEntityToSystem(entity, system.get());
        }

        system->start();
        registeredSystems[systemType] = std::move(system);
    }

    template<typename T>
    void unRegisterSystem() {
        const SystemTypeId systemType = SystemType<T>();
        if (registeredSystems.count(systemType) != 0) {
            throw std::runtime_error("System not register");
        }
        registeredSystems.erase(systemType);
    }

private:
    template<typename T>
    void addCompList() {
        const ComponentTypeId compType = CompType<T>();
        if (componentsArrays.find(compType) != componentsArrays.end()) {
            throw std::runtime_error("Component list already register");
        }
        componentsArrays[compType] = std::move(std::make_shared<CompList<T>>());
    }

    template<typename T>
    std::shared_ptr<CompList<T>> getCompList() {
        const ComponentTypeId compType = CompType<T>();
        if (componentsArrays.count(compType) == 0) {
            addCompList<T>();
        }
        return std::static_pointer_cast<CompList<T>>(componentsArrays.at(compType));
    }

    void addEntitySignature(const EntityId entity) {
        if (entitiesSignatures.find(entity) != entitiesSignatures.end()) {
            throw std::logic_error("error:: add entity");
        }
        entitiesSignatures[entity] = std::move(std::make_shared<Signature>());
    }

    std::shared_ptr<Signature> getEntitySignature(const EntityId entity) {
        if (entitiesSignatures.find(entity) == entitiesSignatures.end()) {
            throw std::logic_error("error:: add entity");
        }
        return entitiesSignatures.at(entity);
    }

    void updateEntityTargetSystems(const EntityId entity) {
        for (auto &system: registeredSystems) {
            addEntityToSystem(entity, system.second.get());
        }
    }

    void addEntityToSystem(const EntityId entity, BaseSystem *system) {
        if (belongToSystem(entity, system->signature)) {
            system->entities.insert(entity);
        } else {
            system->entities.erase(entity);
        }
    }

    bool belongToSystem(const EntityId entity, const Signature &system_signature) {
        for (const auto compType: system_signature) {
            if (getEntitySignature(entity)->count(compType) == 0) {
                return false;
            }
        }
        return true;
    }
};

#endif //CG_ENGINE_ENTITYMANAGER_H
