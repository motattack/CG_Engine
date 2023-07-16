#ifndef CG_ENGINE_NAME_H
#define CG_ENGINE_NAME_H

#include <CG_Engine/base/baseComponent.h>

struct EntityName : public BaseComponent {
    ~EntityName() = default;

    EntityName() : Value("NewEntity") {}

    EntityName(std::string value) : Value(value) {}

    std::string Value;
};

#endif //CG_ENGINE_NAME_H
