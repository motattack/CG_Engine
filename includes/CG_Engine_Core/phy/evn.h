#ifndef CG_ENGINE_EVN_H
#define CG_ENGINE_EVN_H

#include <CG_Engine_Core/math/vec3.h>

class Environment {
public:
    static Vec3 gravitationalAcceleration;
};

Vec3 Environment::gravitationalAcceleration = Vec3(0.0f, -9.81f, 0.0f);

#endif //CG_ENGINE_EVN_H
