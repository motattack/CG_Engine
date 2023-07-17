#ifndef CG_ENGINE_CAMERA_SYSTEM_H
#define CG_ENGINE_CAMERA_SYSTEM_H

#include <CG_Engine/base/baseSystem.h>
#include <CG_Engine/components/transform.h>
#include <CG_Engine/components/camera.h>
#include <CG_Engine/engine.h>
#include <CG_Engine/ui/input.h>
#include <CG_Engine/render/shader.h>
#include <CG_Engine/source.h>

class CameraSystem : public BaseSystem {

public:
    CameraSystem() {
        addComponentSignature<Camera>();
        addComponentSignature<Transform>();

        shader = Shader(Resource.program("MAIN"));
        skyboxShader = Shader(Resource.program("SKYBOX"));
    }

    void update() override {
        for (auto entity: entities) {
            auto &camera = Manager.getComponent<Camera>(entity);
            auto &transform = Manager.getComponent<Transform>(entity);
            camera.Position = transform.Position;
            camera.Yaw = transform.Rotation.x;
            camera.Pitch = transform.Rotation.y;

            event(camera);
            transform.Position = camera.Position;
            transform.Rotation.x = camera.Yaw;
            transform.Rotation.y = camera.Pitch;

//            std::cout << camera.Yaw << std::endl;

            glDepthMask(GL_FALSE);
            skyboxShader.bind();
            skyboxShader.setMat4("projection", camera.GetProjectionMatrix());
            skyboxShader.setMat4("view", camera.GetViewMatrixClear());
            glBindTexture(GL_TEXTURE_CUBE_MAP, Resource.cubeMap("SKYBOX"));
            Resource.getMesh("CUBE").Draw(skyboxShader);
            glDepthMask(GL_TRUE);

            shader.bind();
            shader.setInt("material.diffuse", 0);
            shader.setInt("material.specular", 1);
            shader.setMat4("projection", camera.GetProjectionMatrix());
            shader.setMat4("view", camera.GetViewMatrix());
        }
    }

    void event(Camera &camera) {
        auto &event = Input::Event();
        if (event.isMouseScrolling()) {
            camera.ChangeFOV(event.mouseScrollOffset().y);
        }
        event.resetScrollState();

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            camera.Move(CameraDirection::FORWARD, timer.deltaTime());
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            camera.Move(CameraDirection::BACKWARD, timer.deltaTime());
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            camera.Move(CameraDirection::RIGHT, timer.deltaTime());
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            camera.Move(CameraDirection::LEFT, timer.deltaTime());
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            camera.Move(CameraDirection::UP, timer.deltaTime());
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
            camera.Move(CameraDirection::DOWN, timer.deltaTime());
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            camera.Rotate(-timer.deltaTime() * 360.f, 0);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            camera.Rotate(timer.deltaTime() * 360.f, 0);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            camera.Rotate(0, timer.deltaTime() * 360.f);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            camera.Rotate(0, -timer.deltaTime() * 360.f);
        }

        if (event.isMouseMoving()) {
//            std::cout << event.getDX() << ":" << event.getDY() << std::endl;
            float x = event.getDX(), y = event.getDY();
            camera.Rotate(x, y);
        }
    }

private:
    Shader shader;
    Shader skyboxShader;
};


#endif //CG_ENGINE_CAMERA_SYSTEM_H
