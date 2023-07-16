#ifndef CG_ENGINE_TYPES_H
#define CG_ENGINE_TYPES_H

#include <set>
#include <memory>

class Entity;

class BaseSystem;

class BaseComponent;

class EntityManager;

constexpr int INVALID_TYPE_ID = 0;
constexpr int INVALID_ENTITY = -1;
constexpr int MAX_COMP_COUNT = 32;
constexpr int MAX_ENTITY_COUNT = 300;

using EntityId = int;
using SystemTypeId = int;
using ComponentTypeId = int;
using Signature = std::set<ComponentTypeId>;
using FactoryType = std::shared_ptr<BaseComponent>;

// Runtime type
static int GetRuntimeTypeID() {
    static int typeID = 1u;
    return typeID++;
}

template<typename T>
SystemTypeId SystemType() {
    static_assert((std::is_base_of<BaseSystem, T>::value && !std::is_same<BaseSystem, T>::value),
                  "Invalid type");
    static const SystemTypeId typeId = GetRuntimeTypeID();
    return typeId;
}

template<typename T>
ComponentTypeId CompType() {
    static_assert((std::is_base_of<BaseComponent, T>::value && !std::is_same<BaseComponent, T>::value),
                  "Invalid type");
    static const ComponentTypeId typeId = GetRuntimeTypeID();
    return typeId;
}

#endif //CG_ENGINE_TYPES_H
