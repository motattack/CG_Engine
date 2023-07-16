#ifndef CG_ENGINE_ENTITYMANAGER_H
#define CG_ENGINE_ENTITYMANAGER_H

#include <CG_Engine/base/types.h>
#include <CG_Engine/base/compList.h>
#include <CG_Engine/base/baseSystem.h>
#include <CG_Engine/base/baseComponent.h>
#include <CG_Engine/base/compFactory.h>

#include <map>
#include <queue>
#include <memory>

class EntityManager {
private:
    EntityId entityCount;
    std::set<EntityId> activeEntities;
    std::queue<EntityId> availableEntities;

    std::set<std::shared_ptr<BaseSystem>> systems;
    std::set<std::shared_ptr<BaseSystem>> activeSystems;
    std::set<std::shared_ptr<BaseSystem>> editorSystems;
    std::set<std::shared_ptr<BaseSystem>> runtimeSystems;

    std::set<std::shared_ptr<ICompList>> componentArrays;
    std::unordered_map<EntityId, Signature> entitySignatures;
public:
    ~EntityManager() = default;

    EntityManager(const EntityManager &) = delete;

    EntityManager &operator=(const EntityManager &) = delete;

    static EntityManager &Ref() {
        static EntityManager reference;
        return reference;
    }

    void start() {
        for (auto &system: activeSystems) {
            system->start();
        }
    }

    void update() {
        for (auto &system: activeSystems) {
            system->update();
        }
    }

    void render() {
        for (auto &system: activeSystems) {
            system->render();
        }
    }

    void activateRuntimeSystems() {
        for (auto &system: runtimeSystems) {
            system->start();
            activeSystems.insert(system);
        }
    }

    void activateEditorSystems() {
        for (auto &system: editorSystems) {
            system->start();
            activeSystems.insert(system);
        }
    }

    void deactivateRuntimeSystems() {
        for (auto &system: runtimeSystems) {
            system->stop();
            activeSystems.erase(system);
        }
    }

    void deactivateEditorSystems() {
        for (auto &system: editorSystems) {
            system->stop();
            activeSystems.erase(system);
        }
    }

    const std::set<EntityId> &ActiveEntities() const {
        return activeEntities;
    }

    EntityId addNewEntity() {
        if (entityCount >= MAX_ENTITY_COUNT)
            std::cout << "Entity count limit reached!" << std::endl;

        EntityId entity = availableEntities.front();
        activeEntities.insert(entity);
        availableEntities.pop();
        entityCount++;
        return entity;
    }

    void destroyEntity(const EntityId entity) {
        if (entity >= MAX_ENTITY_COUNT)
            std::cout << "EntityID our of range!" << std::endl;

        entitySignatures.erase(entity);
        for (auto &array: componentArrays)
            array->erase(entity);

        updateEntityTargetSystems(entity);
        entityCount = (entityCount > 0) ? entityCount-- : 0;
        availableEntities.push(entity);
        activeEntities.erase(entity);
    }

    template<typename T, typename... Args>
    void addComponent(const EntityId entity, Args &&... args) {
        if (entity >= MAX_ENTITY_COUNT)
            std::cout << "EntityID out of range" << std::endl;
        if (entitySignatures[entity].size() >= MAX_COMP_COUNT)
            std::cout << "Component count limit reached!" << std::endl;
        if (!isCompListRegistered<T>())
            registerCompList<T>();

        T component(std::forward<Args>(args)...);
        component.entityID = entity;
        entitySignatures[entity].insert(CompType<T>());
        getCompList<T>()->insert(&component);
        updateEntityTargetSystems(entity);
    }

    template<typename T>
    void addComponent(const EntityId entity, T component) {
        if (entity >= MAX_ENTITY_COUNT)
            std::cout << "EntityID out of range" << std::endl;
        if (entitySignatures[entity].size() >= MAX_COMP_COUNT)
            std::cout << "Component count limit reached!" << std::endl;
        if (!isCompListRegistered<T>())
            registerCompList<T>();

        component.entityID = entity;
        getCompList<T>()->insert(&component);
        entitySignatures[entity].insert(CompType<T>());
        updateEntityTargetSystems(entity);
    }

    void addComponent(const EntityId entity, const char *typeName) {
        if (entity >= MAX_ENTITY_COUNT)
            std::cout << "EntityID out of range" << std::endl;
        if (entitySignatures[entity].size() >= MAX_COMP_COUNT)
            std::cout << "Component count limit reached!" << std::endl;
        const ComponentTypeId compType = CompFactory.getTypeId(typeName);
        auto it = std::find_if(componentArrays.begin(), componentArrays.end(),
                               [compType](const auto array) { return compType == array->getDataType(); });
        if (it == componentArrays.end())
            std::cout << "Component list not registered" << std::endl;

        auto component = CompFactory.createComponent(typeName);
        component->entityID = entity;
        (*it)->insert(component.get());
        entitySignatures[entity].insert(compType);
        updateEntityTargetSystems(entity);
    }

    template<typename T>
    void removeComponent(const EntityId entity) {
        if (entity >= MAX_ENTITY_COUNT)
            std::cout << "EntityID out of range" << std::endl;

        entitySignatures[entity].erase(CompType<T>());
        getCompList<T>()->Erase(entity);
        updateEntityTargetSystems(entity);
    }

    void removeComponent(const EntityId entity, const char *typeName) {
        if (entity >= MAX_ENTITY_COUNT)
            std::cout << "EntityID out of range" << std::endl;

        const ComponentTypeId compType = CompFactory.getTypeId(typeName);
        auto it = std::find_if(componentArrays.begin(), componentArrays.end(),
                               [compType](const auto array) { return compType == array->getDataType(); });
        if (it == componentArrays.end())
            std::cout << "Component list not registered" << std::endl;

        (*it)->erase(entity);
        entitySignatures[entity].erase(compType);
        updateEntityTargetSystems(entity);
    }

    template<typename T>
    T &getComponent(const EntityId entity) {
        if (entity >= MAX_ENTITY_COUNT)
            std::cout << "EntityID out of range" << std::endl;

        return getCompList<T>()->get(entity);
    }

    template<typename T>
    bool hasComponent(const EntityId entity) {
        if (entity >= MAX_ENTITY_COUNT)
            std::cout << "EntityID out of range!" << std::endl;

        return (entitySignatures[entity].count(CompType<T>()) > 0);
    }

    template<typename T>
    void addSystem() {
        auto system = std::make_shared<T>();
        systems.insert(std::move(system));
    }

    template<typename T>
    void addRuntimeSystem() {
        auto system = std::make_shared<T>();
        runtimeSystems.insert(std::move(system));
    }

    template<typename T>
    void addEditorSystem() {
        auto system = std::make_shared<T>();
        editorSystems.insert(std::move(system));
    }

    template<typename T>
    void registerCompList() {
        ComponentTypeId compType = CompType<T>();
        auto it = std::find_if(componentArrays.begin(), componentArrays.end(),
                               [compType](const auto array) { return compType == array->getDataType(); });

        if (it != componentArrays.end())
            std::cout << "Component list all ready registered" << std::endl;

        componentArrays.insert(std::move(std::make_shared<CompList<T>>()));
    }

private:
    EntityManager() : entityCount(0), componentArrays({}) {
        for (EntityId entity = 0; entity < MAX_ENTITY_COUNT; entity++)
            availableEntities.push(entity);
    }

    void updateEntityTargetSystems(EntityId entity) {
        for (auto system: systems)
            pushEntityToSystem(entity, system);

        for (auto system: editorSystems)
            pushEntityToSystem(entity, system, false);

        for (auto system: runtimeSystems)
            pushEntityToSystem(entity, system, false);
    }

    void pushEntityToSystem(EntityId entity, std::shared_ptr<BaseSystem> &system, bool activateSystem = true) {
        for (auto &compType: system->getSignature()) {
            if (entitySignatures[entity].count(compType) == 0) {
                system->eraseEntity(entity);
                if (system->isEmpty() && activeSystems.count(system) > 0)
                    activeSystems.erase(system);
                return;
            }
        }

        system->pushEntity(entity);
        if (activateSystem && activeSystems.count(system) == 0)
            activeSystems.insert(system);
    }

    template<typename T>
    bool isCompListRegistered() {
        ComponentTypeId compType = CompType<T>();
        auto it = std::find_if(componentArrays.begin(), componentArrays.end(),
                               [compType](const auto array) { return compType == array->getDataType(); });
        return (it != componentArrays.end());
    }

    template<typename T>
    std::shared_ptr<CompList<T>> getCompList() {
        ComponentTypeId compType = CompType<T>();
        auto it = std::find_if(componentArrays.begin(), componentArrays.end(),
                               [compType](const auto array) { return compType == array->getDataType(); });

        if(it == componentArrays.end())
            std::cout << "Component list not registered" << std::endl;

        return std::static_pointer_cast<CompList<T>>(*it);
    }
};

static EntityManager &Manager = EntityManager::Ref();

#endif //CG_ENGINE_ENTITYMANAGER_H
