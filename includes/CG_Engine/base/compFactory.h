#ifndef CG_ENGINE_COMPFACTORY_H
#define CG_ENGINE_COMPFACTORY_H

#include <functional>
#include <map>
#include <iostream>

using Constructor = std::function<FactoryType()>;

class ComponentFactory {
private:
    ComponentFactory() = default;

    std::map<const char *, Constructor> registryFuncs;
    std::map<const char *, ComponentTypeId> registryTypes;
public:
    ~ComponentFactory() = default;

    ComponentFactory(const ComponentFactory &) = delete;

    ComponentFactory &operator=(const ComponentFactory &) = delete;

    static ComponentFactory &Ref() {
        static ComponentFactory reference;
        return reference;
    }

    auto createComponent(const char *typeName) {
        auto it = registryFuncs.find(typeName);
        if (it == registryFuncs.end())
            std::cout << "Component Type not found in registry" << std::endl;
        auto component = it->second();
        return component;
    }

    void registerType(ComponentTypeId compType, const char *typeName, Constructor callback) {
        registryFuncs[typeName] = callback;
        registryTypes[typeName] = compType;
    }

    const ComponentTypeId getTypeId(const char *typeName) {
        if (registryTypes.find(typeName) == registryTypes.end())
            std::cout << "Typename out of range" << std::endl;

        return registryTypes.at(typeName);
    }
};

static ComponentFactory &CompFactory = ComponentFactory::Ref();

template<typename T>
class Registrar {
public:
    Registrar(const char *typeName) {
        const ComponentTypeId compType = CompType<T>();
        CompFactory.registerType(compType, typeName, []() -> FactoryType { return std::move(std::make_shared<T>()); });
    }
};

#endif //CG_ENGINE_COMPFACTORY_H
