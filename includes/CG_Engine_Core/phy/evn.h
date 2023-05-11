#ifndef CG_ENGINE_EVN_H
#define CG_ENGINE_EVN_H

#include <CG_Engine_Core/math/vec3.h>

// T

/*
    Environment class
    - stores values for environment (constants, etc)

    NOTE: 1 cubic unit in OpenGL space is equivalent to 1 meter cubed
*/

class Environment {
public:
    static Vec3 worldUp;						// up vector in world
    static Vec3 gravitationalAcceleration;		// acceleration due to gravity
};

Vec3 Environment::worldUp = Vec3(0.0f, 1.0f, 0.0f);

// acceleration due to gravity
Vec3 Environment::gravitationalAcceleration = Vec3(0.0f, -9.81f, 0.0f);

#endif //CG_ENGINE_EVN_H
