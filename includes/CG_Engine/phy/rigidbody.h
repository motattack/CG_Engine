#ifndef CG_ENGINE_RIGIDBODY_H
#define CG_ENGINE_RIGIDBODY_H

#include <CG_Engine_Core/math/vec3.h>

#include <cmath>

class RigidBody {
public:
    float mass;

    Vec3 pos;
    Vec3 velocity;
    Vec3 acceleration;

    explicit RigidBody(float mass = 1.0f, Vec3 pos = Vec3(0.0f), Vec3 velocity = Vec3(0.0f),
                       Vec3 acceleration = Vec3(0.0f))
            : mass(mass), pos(pos), velocity(velocity), acceleration(acceleration) {};

    void update(float dt) {
        pos += velocity * dt + acceleration * 0.5f * (dt * dt);
        velocity += acceleration * dt;
    }

    void applyForce(Vec3 force) {
        acceleration += force / mass;
    };

    void applyForce(Vec3 direction, float magnitude) {
        applyForce(direction * magnitude);
    };

    void applyAcceleration(Vec3 accel) {
        acceleration += accel;
    };

    void applyAcceleration(Vec3 direction, float magnitude) {
        applyAcceleration(direction * magnitude);
    };

    void applyImpulse(Vec3 force, float dt) {
        velocity += force / mass * dt;
    };

    void applyImpulse(Vec3 direction, float magnitude, float dt) {
        applyImpulse(direction * magnitude, dt);
    };

    void transferEnergy(float joules, Vec3 direction) {
        if (joules == 0) {
            return;
        }

        // comes from formula: KE = 1/2 * m * v^2
        Vec3 deltaV = direction * std::sqrt(2 * abs(joules) / mass);

        velocity += joules > 0 ? deltaV : -deltaV;
    };

    std::string toString() const {
        return "mass: " + std::to_string(mass);
    }
};


#endif //CG_ENGINE_RIGIDBODY_H
