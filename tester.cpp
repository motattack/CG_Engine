#include <iostream>
#include <CG_Engine/objects/model.h>
#include <CG_Engine/scene.h>

Mat4x4 projection, view;

Vec3 pointLightPositions[] = {
        Vec3(0.7f, 0.2f, 2.0f),
        Vec3(2.3f, -3.3f, -4.0f),
        Vec3(-4.0f, 2.0f, -12.0f),
        Vec3(0.0f, 0.0f, -3.0f)
};

vector<std::string> faces{
        "res/textures/skybox/earth/right.png",
        "res/textures/skybox/earth/left.png",
        "res/textures/skybox/earth/top.png",
        "res/textures/skybox/earth/bottom.png",
        "res/textures/skybox/earth/front.png",
        "res/textures/skybox/earth/back.png"
};

float skyboxVertices[] = {
        -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f
};

int main() {
    Scene scene = Scene("CL_Engine", 960, 540);

    if (!scene.init()) {
        std::cout << "Could not open window" << std::endl;
        return -1;
    }

    /* Shader */
    Shader myShader = Shader("res/shader/vShader.glsl", "res/shader/fShader.glsl");
    Shader skyboxShader = Shader("res/shader/sbox.vert", "res/shader/sbox.frag");

    vArray VAO_SKYBOX;

    vBuffer VBO(skyboxVertices, sizeof(skyboxVertices));
    vArray::attrPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) nullptr);

    unsigned int cubeMapTexture = loadCubeMap(faces);

    sf::Clock clock, deltaClock;
    while (!scene.shouldClose()) {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        skyboxShader.use();
        skyboxShader.setMat4("projection", scene.camera.GetProjectionMatrix());
        skyboxShader.setMat4("view", scene.camera.GetViewMatrixClear());
//        VAO_SKYBOX.bind();
//        glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        scene.window.display();
    }
    scene.cleanup();
    return 0;
}