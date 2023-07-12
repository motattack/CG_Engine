#ifndef CG_ENGINE_SKYBOX_H
#define CG_ENGINE_SKYBOX_H

#include <CG_Engine/objects/model.h>

float verticesCube[] = {
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

int noVerticesCube = 36;

vector<std::string> faces{
        "res/textures/skybox/earth/right.png",
        "res/textures/skybox/earth/left.png",
        "res/textures/skybox/earth/top.png",
        "res/textures/skybox/earth/bottom.png",
        "res/textures/skybox/earth/front.png",
        "res/textures/skybox/earth/back.png"
};

class Cube : public Model {
public:
    Cube() : Model() {
        std::vector<Texture> textures;
        Texture texture;
        texture.id = loadCubeMap(faces);
        texture.path = "res/textures/example";
        texture.type = "texture_diffuse";
        textures.push_back(texture);

        std::vector<unsigned int> indices(noVerticesCube);
        for (unsigned int i = 0; i < noVerticesCube; i++) {
            indices[i] = i;
        }

        Mesh cubeMesh(Vertex::genList(verticesCube, noVerticesCube), indices, textures);
        meshes.push_back(cubeMesh);
    }

    void render(Shader shader) {
        Model::Draw(shader);
    }
};

#endif //CG_ENGINE_SKYBOX_H
