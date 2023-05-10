#ifndef CG_ENGINE_RIGIDBODY_H
#define CG_ENGINE_RIGIDBODY_H

#include <CG_Engine_Core/math/vec3.h>

#include <cmath>
#include <utility>

#define INSTANCE_DEAD        (unsigned char)0b00000001
#define INSTANCE_MOVED        (unsigned char)0b00000010

class RigidBody {
public:
    unsigned char state{};

    float mass{};

    Vec3 pos;
    Vec3 velocity;
    Vec3 acceleration;

    Vec3 size;

    std::string modelId;
    std::string instanceId;

    bool operator==(RigidBody rb) const {
        return instanceId == rb.instanceId;
    };

    bool operator==(std::string id) const {
        return instanceId == id;
    };

    RigidBody() = default;

    explicit RigidBody(std::string modelId, Vec3 size = Vec3(1.0f), float mass = 1.0f, Vec3 pos = Vec3(0.0f)) : modelId(
            std::move(modelId)),
                                                                                                                size(size),
                                                                                                                mass(mass),
                                                                                                                pos(pos),
                                                                                                                velocity(
                                                                                                                        0.0f),
                                                                                                                acceleration(
                                                                                                                        0.0f),
                                                                                                                state(0) {}

    void update(float dt) {
        pos += velocity * dt + acceleration * 0.5f * (dt * dt);
        velocity += acceleration * dt;
    };

    void applyForce(Vec3 force) {
        acceleration += force / mass;
    };

    void applyForce(Vec3 direction, float magnitude) {
        applyForce(direction * magnitude);
    };

    void applyAcceleration(Vec3 a) {
        acceleration += a;
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
};


#endif //CG_ENGINE_RIGIDBODY_H
