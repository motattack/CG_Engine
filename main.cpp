#include <iostream>
#include <CG_Engine/objects/model.h>
#include <CG_Engine/models/cube.h>
#include <CG_Engine/scene.h>
#include <CG_Engine/gui.h>

Mat4x4 projection, model, view;

Vec3 pointLightPositions[] = {
        Vec3(0.7f, 0.2f, 2.0f),
        Vec3(2.3f, -3.3f, -4.0f),
        Vec3(-4.0f, 2.0f, -12.0f),
        Vec3(0.0f, 0.0f, -3.0f)
};

int main() {
    Scene scene = Scene("CL_Engine", 1280, 750);

    if (!scene.init()) {
        std::cout << "Could not open window" << std::endl;
        return -1;
    }

    /* Shader */
    Shader myShader("res/shader/vShader.glsl", "res/shader/fShader.glsl");
    Shader lightCubeShader("res/Shader/lCube.vert", "res/Shader/lCube.frag");

    Cube superCube, lightCube;
    Model myBackPack("res/models/backpack/backpack.obj");

    float dt, lastFrame = 0.0f;

    sf::Clock clock, deltaClock;
    while (!scene.shouldClose()) {
        float currentTime = clock.getElapsedTime().asSeconds();
        dt = currentTime - lastFrame;
        lastFrame = currentTime;
        scene.processInput(dt);

        float xValue = std::cos(currentTime) / 2.0f + 0.5f; // 0.0f - 1.0f

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // First Object
        myShader.use();

        // Directional Light
        myShader.set3Float("dirLight.direction", Vec3(dir_direction[0], dir_direction[1], dir_direction[2]));
        myShader.set3Float("dirLight.ambient", Vec3(dir_ambient[0], dir_ambient[1], dir_ambient[2]));
        myShader.set3Float("dirLight.diffuse", Vec3(dir_diffuse[0], dir_ambient[1], dir_ambient[2]));
        myShader.set3Float("dirLight.specular", Vec3(dir_specular[0], dir_ambient[1], dir_ambient[2]));

        // Point Lights
        for (int i = 0; i < 4; i++) {
            myShader.set3Float("pointLights[" + std::to_string(i) + "].position",
                               pointLightPositions[i] * xValue);
            myShader.set3Float("pointLights[" + std::to_string(i) + "].ambient",
                               Vec3(point_ambient[0], point_ambient[1], point_ambient[2]));
            myShader.set3Float("pointLights[" + std::to_string(i) + "].diffuse",
                               Vec3(point_diffuse[0], point_diffuse[1], point_diffuse[2]));
            myShader.set3Float("pointLights[" + std::to_string(i) + "].specular",
                               Vec3(point_specular[0], point_specular[1], point_specular[2]));
            myShader.setFloat("pointLights[" + std::to_string(i) + "].constant", point_constant);
            myShader.setFloat("pointLights[" + std::to_string(i) + "].linear", point_linear);
            myShader.setFloat("pointLights[" + std::to_string(i) + "].quadratic", point_quadratic);
        }

        // Spotlight
        myShader.set3Float("spotLight.position", scene.camera.Position);
        myShader.set3Float("spotLight.direction", scene.camera.Front);
        myShader.set3Float("viewPos", scene.camera.Position);
        myShader.set3Float("spotLight.ambient", Vec3(spot_ambient[0], spot_ambient[1], spot_ambient[2]));
        myShader.set3Float("spotLight.diffuse", Vec3(spot_diffuse[0], spot_diffuse[1], spot_diffuse[2]));
        myShader.set3Float("spotLight.specular", Vec3(spot_specular[0], spot_specular[1], spot_specular[2]));
        myShader.setFloat("spotLight.constant", spot_constant);
        myShader.setFloat("spotLight.linear", spot_linear);
        myShader.setFloat("spotLight.quadratic", spot_quadratic);

        if (scene.getTorch()) {
            myShader.setFloat("spotLight.cutOff", cos(radians(spot_cutOff)));
            myShader.setFloat("spotLight.outerCutOff", cos(radians(spot_outerCutOff)));
        } else {
            myShader.setFloat("spotLight.cutOff", cos(radians(0.0f)));
            myShader.setFloat("spotLight.outerCutOff", cos(radians(0.0f)));

        }

        // Material
        myShader.setFloat("material.shininess", 64.0f);

        /* Coordinates */
        // Projection
        projection = scene.camera.GetProjectionMatrix();
        myShader.setMat4("projection", projection);

        // View
        view = scene.camera.GetViewMatrix();
        myShader.setMat4("view", view);

        // We Draw a map with three loop cuz we are rendering in 3D so each axes need a loop like (x -> i, y -> j, z -> k).
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                for (int k = 0; k < 10; k++) {
                    // We will set blocks like minecraft, we make it to a hollow huge Maps with 10x10x10 cubes
                    if (i == 0 || i == 9 || j == 0 || j == 9 || k == 9) {
                        // Model
                        model = Mat4x4(1.0f);
                        model = model.translate(Vec3(static_cast<float>(i) + cubeMapPos[0],
                                                     static_cast<float>(j) + cubeMapPos[1],
                                                     static_cast<float>(k) + cubeMapPos[2]));
                        //model = model.rotate(radians(-55.0f) * currentTime, Vec3(0.0f, 1.0f, 0.0f));

                        myShader.setMat4("model", model);
                        superCube.Draw(myShader);
                    }
                }
            }
        }

        model = Mat4x4(1.0f);
        model = model.translate(Vec3(2.f));
        myShader.setMat4("model", model);

        superCube.Draw(myShader);

        model = Mat4x4(1.0f);
        model = model.translate(Vec3(3.f));
        myShader.setMat4("model", model);

        myBackPack.Draw(myShader);

        // Second Object
        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);


        for (auto pointLightPosition: pointLightPositions) {
            // Model
            model = Mat4x4(1.0f);
            model = model.translate(pointLightPosition * xValue);
            model = model.scale(Vec3(0.3f));
            lightCubeShader.setMat4("model", model);
            lightCube.Draw(lightCubeShader);
        }
        vArray::unbind();

        light_editor(scene, deltaClock);

        scene.newFrame();

        scene.PollEvents();
    }
    scene.cleanup();
    return 0;
}