#ifndef CG_ENGINE_PLANE_H
#define CG_ENGINE_PLANE_H

#include <CG_Engine/objects/model.h>

int noVerticesPlane = 6;

float verticesPlane[] = {
// position normal texCoord
        -0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f

};

class Plane : public Model {
public:
    Plane() : Model() {
        std::vector<Texture> textures;
        Texture texture;
        texture.id = TextureFromFile("plane_texture.png", "res/textures/example");
        texture.path = "res/textures/example";
        texture.type = "texture_diffuse";
        textures.push_back(texture);

        std::vector<unsigned int> indices(noVerticesPlane);
        for (unsigned int i = 0; i < noVerticesPlane; i++) {
            indices[i] = i;
        }

        Mesh planeMesh(Vertex::genList(verticesPlane, noVerticesPlane), indices, textures);
        meshes.push_back(planeMesh);
    }

    void render(Shader shader) {
        Model::Draw(shader);
    }

};

#endif //CG_ENGINE_PLANE_H
