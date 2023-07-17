#ifndef CG_ENGINE_INPUT_SYSTEM_H
#define CG_ENGINE_INPUT_SYSTEM_H

#include <CG_Engine/base/baseSystem.h>
#include <CG_Engine/components/transform.h>
#include <CG_Engine/components/keyMotion.h>


class KeyMotionSystem : public BaseSystem {
public:
    KeyMotionSystem() {
        addComponentSignature<Transform>();
        addComponentSignature<KeyMotion>();
    }

    void update() override {
        float deltatime = timer.deltaTime();

        for (auto entity: entities) {
            auto &transform = Manager.getComponent<Transform>(entity);
            auto &keyMotion = Manager.getComponent<KeyMotion>(entity);

            if (keyMotion.axis == Action_Type::WASD) {
                WASDControl(transform, keyMotion.speed, deltatime);
                continue;
            }
            if (keyMotion.axis == Action_Type::WASDQE) {
                WASDQEControl(transform, keyMotion.speed, deltatime);
                continue;
            }
            if (keyMotion.axis == Action_Type::ARROWS) {
                ARROWSControl(transform, keyMotion.speed, deltatime);
                std::cout << "arrows";
                continue;
            }
            if (keyMotion.axis == Action_Type::NUMERIC_ARROWS) {
                NUMControl(transform, keyMotion.speed, deltatime);
                continue;
            }
        }
    }

    void ARROWSControl(Transform &transform, float speed, float deltatime) {
        auto &event = Input::Event();

        // X-AXIS
        if (event.IsKeyPressed(sf::Keyboard::Right)) {
            transform.Position.x += speed * deltatime;
        }
        if (event.IsKeyPressed(sf::Keyboard::Left)) {
            transform.Position.x -= speed * deltatime;
        }

        // Y-AXIS
        if (event.IsKeyPressed(sf::Keyboard::Up)) {
            transform.Position.y += speed * deltatime;
        }
        if (event.IsKeyPressed(sf::Keyboard::Down)) {
            transform.Position.y -= speed * deltatime;
        }
    }

    void WASDControl(Transform &transform, float speed, float deltatime) {
        auto &event = Input::Event();
        // X-AXIS
        if (event.IsKeyPressed(sf::Keyboard::D)) {
            transform.Position.x += speed * deltatime;
        }
        if (event.IsKeyPressed(sf::Keyboard::A)) {
            transform.Position.x -= speed * deltatime;
        }

        // Y-AXIS
        if (event.IsKeyPressed(sf::Keyboard::W)) {
            transform.Position.y += speed * deltatime;
        }
        if (event.IsKeyPressed(sf::Keyboard::S)) {
            transform.Position.y -= speed * deltatime;
        }
    }

    void WASDQEControl(Transform &transform, float speed, float deltatime) {
        auto &event = Input::Event();
        // X-AXIS
        if (event.IsKeyPressed(sf::Keyboard::D)) {
            transform.Position.x += speed * deltatime;
        }
        if (event.IsKeyPressed(sf::Keyboard::A)) {
            transform.Position.x -= speed * deltatime;
        }

        // Y-AXIS
        if (event.IsKeyPressed(sf::Keyboard::W)) {
            transform.Position.y += speed * deltatime;
        }
        if (event.IsKeyPressed(sf::Keyboard::S)) {
            transform.Position.y -= speed * deltatime;
        }

        // Z-AXIS
        if (event.IsKeyPressed(sf::Keyboard::Q)) {
            transform.Position.z += speed * deltatime;
        }
        if (event.IsKeyPressed(sf::Keyboard::E)) {
            transform.Position.z -= speed * deltatime;
        }
    }

    void NUMControl(Transform &transform, float speed, float deltatime) {
        auto &event = Input::Event();

        // X-AXIS
        if (event.IsKeyPressed(sf::Keyboard::Num6)) {
            transform.Position.x += speed * deltatime;
        }
        if (event.IsKeyPressed(sf::Keyboard::Num4)) {
            transform.Position.x -= speed * deltatime;
        }

        // Y-AXIS
        if (event.IsKeyPressed(sf::Keyboard::Num8)) {
            transform.Position.y += speed * deltatime;
        }
        if (event.IsKeyPressed(sf::Keyboard::Num2)) {
            transform.Position.y -= speed * deltatime;
        }
    }
};

#endif //CG_ENGINE_INPUT_SYSTEM_H
