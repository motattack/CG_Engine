#include <iostream>
#include <CG_Engine/objects/model.h>
#include <CG_Engine/models/cube.h>
#include <CG_Engine/models/plane.h>
#include <CG_Engine/scene.h>
#include <CG_Engine/gui.h>

Mat4x4 projection, model, view;

Vec3 pointLightPositions[] = {
        Vec3(0.7f, 0.2f, 2.0f),
        Vec3(2.3f, -3.3f, -4.0f),
        Vec3(-4.0f, 2.0f, -12.0f),
        Vec3(0.0f, 0.0f, -3.0f)
};

//vector<std::string> faces{
//        "res/textures/skybox/tron/right.png",
//        "res/textures/skybox/tron/left.png",
//        "res/textures/skybox/tron/top.png",
//        "res/textures/skybox/tron/bottom.png",
//        "res/textures/skybox/tron/front.png",
//        "res/textures/skybox/tron/back.png"
//};

//vector<std::string> faces{
//        "res/textures/skybox/cave/right.png",
//        "res/textures/skybox/cave/left.png",
//        "res/textures/skybox/cave/top.png",
//        "res/textures/skybox/cave/bottom.png",
//        "res/textures/skybox/cave/front.png",
//        "res/textures/skybox/cave/back.png"
//};

vector<std::string> faces{
        "res/textures/skybox/earth/right.png",
        "res/textures/skybox/earth/left.png",
        "res/textures/skybox/earth/top.png",
        "res/textures/skybox/earth/bottom.png",
        "res/textures/skybox/earth/front.png",
        "res/textures/skybox/earth/back.png"
};

//vector<std::string> faces{
//        "res/textures/skybox/sea/right.jpg",
//        "res/textures/skybox/sea/left.jpg",
//        "res/textures/skybox/sea/top.jpg",
//        "res/textures/skybox/sea/bottom.jpg",
//        "res/textures/skybox/sea/front.jpg",
//        "res/textures/skybox/sea/back.jpg"
//};


unsigned int loadCubemap(vector<std::string> faces) {
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++) {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        } else {
            std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
}



int main() {
    Scene scene = Scene("CL_Engine", 1280, 750);

    if (!scene.init()) {
        std::cout << "Could not open window" << std::endl;
        return -1;
    }

    /* Shader */
    Shader myShader("res/shader/vShader.glsl", "res/shader/fShader.glsl");
    Shader skyboxShader("res/shader/sbox.vert", "res/shader/sbox.frag");
    skyboxShader.use();
    myShader.use();
    myShader.setInt("material.diffuse", 0);
    myShader.setInt("material.specular", 1);
    Shader lightCubeShader("res/Shader/lCube.vert", "res/Shader/lCube.frag");

    Cube superCube, lightCube;
    scene.manager.baseAddModel(superCube, "Cube");

    Plane plane;
    scene.manager.baseAddModel(plane, "Plane");

    Model Extinguisher("res/models/amc/scene.gltf");
    scene.manager.baseAddModel(Extinguisher, "Extinguisher");

    Model sphere("res/models/sphere/untitled.obj");
    scene.manager.baseAddModel(sphere, "Sphere");

    float skyboxVertices[] = {
// positions
            -1.0f,  1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f, -1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,

            -1.0f, -1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f, -1.0f,  1.0f,
            -1.0f, -1.0f,  1.0f,

            -1.0f,  1.0f, -1.0f,
            1.0f,  1.0f, -1.0f,
            1.0f,  1.0f,  1.0f,
            1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f,  1.0f,
            -1.0f,  1.0f, -1.0f,

            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f,  1.0f,
            1.0f, -1.0f,  1.0f
    };

    vArray VAO_SKYBOX;

    vBuffer VBO(skyboxVertices, sizeof(skyboxVertices));
    vArray::attrPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) nullptr);

    unsigned int cubemapTexture = loadCubemap(faces);


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

        // skybox

        glDepthMask(GL_FALSE);
        skyboxShader.use();
        projection = scene.camera.GetProjectionMatrix();
        skyboxShader.setMat4("projection", projection);
        view = scene.camera.GetViewMatrixClear();
        skyboxShader.setMat4("view", view);
        VAO_SKYBOX.bind();
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glDepthMask(GL_TRUE);

        // First Object
        myShader.use();

        DirLight dir = DirLight(dir_direction, dir_ambient, dir_diffuse, dir_specular);
        dir.render(myShader, 0);


        PointLight point(pointLightPositions[0], point_constant, point_linear, point_quadratic, point_ambient,
                         point_diffuse, point_specular);

        // Point Lights
        for (int i = 0; i < 4; i++) {
            point.position = pointLightPositions[i] * xValue;
            point.render(myShader, i);
        }
//

        SpotLight spot(scene.camera.Position, scene.camera.Front, 0.0f, 0.0f, spot_constant,
                       spot_linear, spot_quadratic, spot_ambient, spot_diffuse, spot_specular, scene.camera.Position);

        if (scene.getTorch()) {
            spot.cutOff = cos(radians(spot_cutOff));
            spot.outerCutOff = cos(radians(spot_outerCutOff));
        } else {
            spot.cutOff = cos(radians(0.0f));
            spot.outerCutOff = cos(radians(0.0f));
        }
        // Material
        myShader.setFloat("material.shininess", 64.0f);
        spot.render(myShader, 0);

        /* Coordinates */
        // Projection
        projection = scene.camera.GetProjectionMatrix();
        myShader.setMat4("projection", projection);

        // View
        view = scene.camera.GetViewMatrix();
        myShader.setMat4("view", view);

        // Manager objects
        scene.manager.renderObjects(myShader);

        // Second Object
        lightCubeShader.use();
        lightCubeShader.setMat4("projection", projection);
        lightCubeShader.setMat4("view", view);
        lightCubeShader.set3Float("light.ambient", Vec3(point_ambient));
        lightCubeShader.set3Float("light.diffuse", Vec3(point_diffuse));


        for (auto pointLightPosition: pointLightPositions) {
            // Model
            model = Mat4x4(1.0f);
            model = model.translate(pointLightPosition * xValue);
            model = model.scale(Vec3(0.3f));
            lightCubeShader.setMat4("model", model);
            lightCube.Draw(lightCubeShader);
        }
        vArray::unbind();

        ImGui::SFML::Update(scene.window, deltaClock.restart());
        ImGui::ShowDemoWindow();
        light_editor(scene);
        ModelsManager(scene.manager);
        ObjectManager(scene.manager);
        WidgetMaterial(scene.manager);
        ImGui::SFML::Render(scene.window);

        glEnable(GL_DEPTH_TEST);

        scene.newFrame();

        scene.PollEvents();
    }
    scene.cleanup();
    return 0;
}