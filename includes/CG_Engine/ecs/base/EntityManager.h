#ifndef CG_ENGINE_ENTITYMANAGER_H
#define CG_ENGINE_ENTITYMANAGER_H

#include <CG_Engine/ecs/base/baseComponent.h>
#include <CG_Engine/ecs/base/baseSystem.h>
#include <CG_Engine/ecs/base/compList.h>
#include <map>
#include <queue>
#include <memory>

class EntityManager {
private:
    EntityId entityCount;
    std::queue<EntityId> availableEntities;
    std::map<EntityId, EntitySignature> entitiesSignatures;
    std::map<SystemTypeId, std::unique_ptr<BaseSystem>> registeredSystems;
    std::map<ComponentTypeId, std::shared_ptr<ICompList>> componentsArrays;
public:
    EntityManager() : entityCount(0) {
        for (EntityId entity = 0; entity < MAX_ENTITY_COUNT; entity++) {
            availableEntities.push(entity);
        }
    }

    ~EntityManager() {

    }

    EntityId addNewEntity() {
        const EntityId entityId = availableEntities.front();
        availableEntities.pop();
        entityCount++;
        return entityId;
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

};

#endif //CG_ENGINE_ENTITYMANAGER_H
