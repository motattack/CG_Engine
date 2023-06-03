#ifndef CG_ENGINE_MODEL_H
#define CG_ENGINE_MODEL_H

#include <CG_Engine_Core/objects/mesh.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <stb_image.h>

#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <CG_Engine_Core/phy/rigidbody.h>

class Model {
public:
    RigidBody rb;
    Vec3 size;

    BoundTypes boundType;

    std::vector<Mesh> meshes;

    explicit Model(BoundTypes boundType = BoundTypes::AABB, Vec3 pos = Vec3(0.0f), Vec3 size = Vec3(1.0f), bool noTex = false)
            : boundType(boundType), size(size), noTex(noTex) {
        rb.pos = pos;
    };

    void loadModel(const std::string &path) {
        Assimp::Importer import;
        const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::cout << "Could not load model at " << path << std::endl << import.GetErrorString() << std::endl;
            return;
        }

        directory = path.substr(0, path.find_last_of('/'));

        processNode(scene->mRootNode, scene);
    };

    void render(Shader shader, float dt, Box *box, bool setModel = true, bool doRender = true) {
        rb.update(dt);

        if (setModel) {
            auto model = Mat4x4(1.0f);
            model = model.translate(rb.pos);
            model = model.scale(size);
            shader.setMat4("model", model);
        }

        shader.setFloat("material.shininess", 0.5f);

        for (auto &mesh: meshes) {
            mesh.render(shader, rb.pos, size, box, doRender);
        }
    };

    void cleanup() {
        for (auto &mesh: meshes) {
            mesh.cleanup();
        }
    }

protected:
    bool noTex;

    std::string directory;

    std::vector<Texture> textures_loaded;

    void processNode(aiNode *node, const aiScene *scene) {
        // process all meshes
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            meshes.push_back(processMesh(mesh, scene));
        }

        // process all child nodes
        for (unsigned int i = 0; i < node->mNumChildren; i++) {
            processNode(node->mChildren[i], scene);
        }
    };

    Mesh processMesh(aiMesh *mesh, const aiScene *scene) {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        BoundingRegion br(boundType);
        Vec3 min((float) (~0));        // min point = max float
        Vec3 max(-(float) (~0));    // max point = min float

        // vertices
        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            Vertex vertex;

            // position
            vertex.pos = Vec3(
                    mesh->mVertices[i].x,
                    mesh->mVertices[i].y,
                    mesh->mVertices[i].z
            );

            for (int j = 0; j < 3; j++) {
                // if smaller than min
                if (vertex.pos[j] < min[j]) min[j] = vertex.pos[j];
                // if larger than max
                if (vertex.pos[j] > max[j]) max[j] = vertex.pos[j];
            }

            // normal vectors
            vertex.normal = Vec3(
                    mesh->mNormals[i].x,
                    mesh->mNormals[i].y,
                    mesh->mNormals[i].z
            );

            // textures
            if (mesh->mTextureCoords[0]) {
                vertex.texCoord = Vec2(
                        mesh->mTextureCoords[0][i].x,
                        mesh->mTextureCoords[0][i].y
                );
            } else {
                vertex.texCoord = Vec2(0.0f);
            }

            vertices.push_back(vertex);
        }

        // process min/max for BR
        if (boundType == BoundTypes::AABB) {
            // assign max and min
            br.min = min;
            br.max = max;
        } else {
            // calculate max distance from the center
            br.center = BoundingRegion(min, max).calculateCenter();
            float maxRadiusSquared = 0.0f;

            for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
                float radiusSquared = 0.0f; // distance for this vertex
                for (int j = 0; j < 3; j++) {
                    radiusSquared += (vertices[i].pos[j] - br.center[j]) * (vertices[i].pos[j] - br.center[j]);
                }
                if (radiusSquared > maxRadiusSquared) {
                    // found new squared radius
                    // a^2 > b^2 --> |a| > |b|
                    maxRadiusSquared = radiusSquared;
                }
            }

            br.radius = std::sqrt(maxRadiusSquared);
        }

        // process indices
        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                indices.push_back(face.mIndices[j]);
            }
        }

        // process material
        if (mesh->mMaterialIndex >= 0) {
            aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

            if (noTex) {
                // diffuse color
                aiColor4D diff(1.0f);
                aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diff);

                // specular color
                aiColor4D spec(1.0f);
                aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &spec);

                return {br, vertices, indices, diff, spec};
            }

            // diffuse maps
            std::vector<Texture> diffuseMaps = loadTextures(material, aiTextureType_DIFFUSE);
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

            // specular maps
            std::vector<Texture> specularMaps = loadTextures(material, aiTextureType_SPECULAR);
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        }

        return Mesh(br, vertices, indices, textures);
    };

    std::vector<Texture> loadTextures(aiMaterial *mat, aiTextureType type) {
        std::vector<Texture> textures;

        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
            aiString str;
            mat->GetTexture(type, i, &str);

            // prevent duplicate loading
            bool skip = false;
            for (auto & j : textures_loaded) {
                if (std::strcmp(j.path.data(), str.C_Str()) == 0) {
                    textures.push_back(j);
                    skip = true;
                    break;
                }
            }

            if (!skip) {
                // not loaded yet
                Texture tex(directory, str.C_Str(), type);
                std::cout << str.C_Str() << std::endl;
                tex.load(false);
                textures.push_back(tex);
                textures_loaded.push_back(tex);
            }
        }

        return textures;
    };
};

#endif //CG_ENGINE_MODEL_H
