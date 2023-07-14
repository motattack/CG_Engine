#ifndef CG_ENGINE_KEYMOTION_H
#define CG_ENGINE_KEYMOTION_H

#include <CG_Engine/base/baseComponent.h>

enum class Action_Type {
    ARROWS = 0, WASD = 1, WASDQE = 2, NUMERIC_ARROWS = 3
};

struct KeyMotion : public BaseComponent {
    float speed;
    Action_Type axis;

    KeyMotion(Action_Type axis = Action_Type::WASDQE, float speed = 5.0f) : speed(speed), axis(axis) {}
};

#endif //CG_ENGINE_KEYMOTION_H
