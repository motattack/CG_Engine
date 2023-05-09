#ifndef CG_ENGINE_MODEL_H
#define CG_ENGINE_MODEL_H

#include <CG_Engine_Core/mesh.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <stb_image.h>

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <CG_Engine_Core/physics/rigidbody.h>

class Model {
public:
    RigidBody rb;
    Vec3 size;

    Model() = default;

    explicit Model(Vec3 pos = Vec3(0.0f), Vec3 size = Vec3(1.0f), bool noTex = false) : size(size), noTex(noTex) {
        rb.pos = pos;
    };

    void loadModel(const std::string& path) {
        Assimp::Importer import;
        const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::cout << "Could not load model at " << path << std::endl << import.GetErrorString() << std::endl;
            return;
        }

        directory = path.substr(0, path.find_last_of('/'));

        processNode(scene->mRootNode, scene);
    };

    void render(Shader shader, float dt, bool setModel = true) {
        rb.update(dt);

        if (setModel) {
            auto model = Mat4x4(1.0f);
            model = model.translate(rb.pos);
            model = model.Scale(size);
            shader.setMat4("model", model);
        }

        shader.setFloat("material.shininess", 0.5f);

        for (auto &mesh: meshes) {
            mesh.render(shader);
        }
    };

    void cleanup() {
        for (auto &mesh: meshes) {
            mesh.cleanup();
        }
    };

protected:
    bool noTex;

    std::vector<Mesh> meshes;
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
    }

    Mesh processMesh(aiMesh *mesh, const aiScene *scene) {
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;

        // vertices
        for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
            Vertex vertex;

            // position
            vertex.pos = Vec3(
                    mesh->mVertices[i].x,
                    mesh->mVertices[i].y,
                    mesh->mVertices[i].z
            );

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

                return {vertices, indices, diff, spec};
            }

            // diffuse maps
            std::vector<Texture> diffuseMaps = loadTextures(material, aiTextureType_DIFFUSE);
            textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

            // specular maps
            std::vector<Texture> specularMaps = loadTextures(material, aiTextureType_SPECULAR);
            textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        }

        return {vertices, indices, textures};
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
                tex.load(false);
                textures.push_back(tex);
                textures_loaded.push_back(tex);
            }
        }

        return textures;
    };
};

#endif //CG_ENGINE_MODEL_H
