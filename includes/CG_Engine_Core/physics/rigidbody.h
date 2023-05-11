#ifndef CG_ENGINE_RIGIDBODY_H
#define CG_ENGINE_RIGIDBODY_H

#include <CG_Engine_Core/math/vec3.h>

#include <cmath>
#include <utility>
#include <string>

// T

// switches for instance states
#define INSTANCE_DEAD        (unsigned char)0b00000001
#define INSTANCE_MOVED        (unsigned char)0b00000010

/*
    Rigid Body class
    - represents physical body and holds all parameters
*/

class RigidBody {
public:
    // combination of switches above
    unsigned char state;

    // mass in kg
    float mass;

    // position in m
    Vec3 pos;
    // velocity in m/s
    Vec3 velocity;
    // acceleration in m/s^2
    Vec3 acceleration;

    // dimensions of object
    Vec3 size;

    // ids for quick access to instance/model
    std::string modelId;
    std::string instanceId;

    // test for equivalence of two rigid bodies
    bool operator==(RigidBody rb) {
        return instanceId == rb.instanceId;
    };

    bool operator==(std::string id) {
        return instanceId == id;
    };

    /*
        constructor
    */

    // construct with parameters and default
    RigidBody(std::string modelId = "", Vec3 size = Vec3(1.0f), float mass = 1.0f,
              Vec3 pos = Vec3(0.0f)) : modelId(modelId), size(size), mass(mass), pos(pos), velocity(0.0f),
                                       acceleration(0.0f), state(0) {};

    /*
        transformation functions
    */

    // update position with velocity and acceleration
    void update(float dt) {
        pos += velocity * dt + acceleration * 0.5f * (dt * dt);
        velocity += acceleration * dt;
    };

    // apply a force
    void applyForce(Vec3 force) {
        acceleration += force / mass;
    };

    void applyForce(Vec3 direction, float magnitude) {
        applyForce(direction * magnitude);
    };

    // apply an acceleration (remove redundancy of dividing by mass)
    void applyAcceleration(Vec3 a) {
        acceleration += a;
    };

    void applyAcceleration(Vec3 direction, float magnitude) {
        applyAcceleration(direction * magnitude);
    };

    // apply force over time
    void applyImpulse(Vec3 force, float dt) {
        velocity += force / mass * dt;
    };

    void applyImpulse(Vec3 direction, float magnitude, float dt) {
        applyImpulse(direction * magnitude, dt);
    };

    // transfer potential or kinetic energy from another object
    void transferEnergy(float joules, Vec3 direction) {
        if (joules == 0) {
            return;
        }

        // comes from formula: KE = 1/2 * m * v^2
        Vec3 deltaV = direction * std::sqrt(2 * abs(joules) / mass);

        velocity += joules > 0 ? deltaV : -deltaV;
    };
};

#endif //CG_ENGINE_RIGIDBODY_H
