#ifndef CG_ENGINE_TYPES_H
#define CG_ENGINE_TYPES_H

#include <set>

class System;

class Component;

const int MAX_ENTITY_COUNT = 500;
const int MAX_COMPONENT_COUNT = 32;

using EntityId = int;
using SystemTypeId = int;
using ComponentTypeId = int;
using EntitySignature = std::set<ComponentTypeId>;

static ComponentTypeId GetComponentTypeId() {
    static ComponentTypeId typeId = 0;
    return typeId++;
}

static const SystemTypeId GetSystemTypeId() {
    static SystemTypeId typeId = 0;
    return typeId++;
}

template<typename T>
static const ComponentTypeId CompType() {
    static_assert((std::is_base_of<Component, T>::value && !std::is_same<Component, T>::value), "ERROR: Invalid component template type");
    static const ComponentTypeId typeId = GetComponentTypeId();
    return typeId;
}

template<typename T>
static const SystemTypeId SystemType() {
    static_assert((std::is_base_of<System, T>::value && !std::is_same<System, T>::value), "ERROR: Invalid system template type");
    static const SystemTypeId typeId = GetSystemTypeId();
    return typeId;
}

#endif //CG_ENGINE_TYPES_H
