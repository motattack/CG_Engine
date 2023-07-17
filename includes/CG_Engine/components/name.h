#ifndef CG_ENGINE_NAME_H
#define CG_ENGINE_NAME_H

struct EntityName : public BaseComponent {
    ~EntityName() = default;

    EntityName() : Value("NewEntity") {}

    EntityName(std::string value) : Value(value) {}

    std::string Value;
};

#endif //CG_ENGINE_NAME_H
