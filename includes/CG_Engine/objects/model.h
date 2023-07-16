#ifndef CG_ENGINE_MODEL_H
#define CG_ENGINE_MODEL_H

const unsigned int FAIL_LOAD_TEXTURE = 3347;

#include <CG_Engine/objects/mesh.h>

#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/pbrmaterial.h>
#include <assimp/postprocess.h>

#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include "CG_Engine/math/common.h"

class Model3D {
private:
    std::string directory;
    std::vector<GLMesh> meshes;
    std::map<const char *, GLuint> loadedTexMaps;
public:
    Model3D(std::string filename) {
        load(filename);
    }

    ~Model3D() {
        for (auto texture: loadedTexMaps) {
            glDeleteTextures(1, &texture.second);
        }
    }

    void render(Shader &shader) {
        for (auto mesh: meshes) {
            mesh.render(shader, GL_TRIANGLES);
        }
    }

private:
    void load(std::string filename) {
        Assimp::Importer loader;
        directory = filename.substr(0, filename.find_last_of('/')).append("/");
        const aiScene *scene = loader.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs);
        if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::cout << "ERROR::ASSIMP::" << loader.GetErrorString() << std::endl;
            return;
        }
        parseNode(scene->mRootNode, scene);
    }

    void parseNode(aiNode *node, const aiScene *scene) {
        for (GLuint i = 0; i < node->mNumMeshes; i++) {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            parseMesh(mesh, scene);
        }

        for (GLuint i = 0; i < node->mNumChildren; i++) {
            parseNode(node->mChildren[i], scene);
        }
    }

    void parseMesh(aiMesh *mesh, const aiScene *scene) {

        std::vector<GLuint> indices;
        std::vector<Vertex> vertices;

        // vertices
        for (GLuint i = 0; i < mesh->mNumVertices; i++) {
            Vertex vertex;
            vertex.Position.x = mesh->mVertices[i].x;
            vertex.Position.y = mesh->mVertices[i].y;
            vertex.Position.z = mesh->mVertices[i].z;

            // normals
            if (mesh->HasNormals()) {
                vertex.Normal.x = mesh->mNormals[i].x;
                vertex.Normal.y = mesh->mNormals[i].y;
                vertex.Normal.z = mesh->mNormals[i].z;
            }

            // texture coordinates
            if (mesh->mTextureCoords[0]) {
                vertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
                vertex.TexCoords.y = mesh->mTextureCoords[0][i].y;
            }

            vertices.push_back(vertex);
        }

        // indices
        for (GLuint i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for (GLuint j = 0; j < face.mNumIndices; j++) {
                indices.push_back(face.mIndices[j]);
            }
        }

        GLMesh glMesh(vertices.data(), vertices.size(), indices.data(), indices.size());

        // material
        if (mesh->mMaterialIndex >= 0) {
            aiMaterial *aMaterial = scene->mMaterials[mesh->mMaterialIndex];
            glMesh.SetMaterial(parseMaterial(aMaterial));
        }

        meshes.push_back(glMesh);
    }

    Material parseMaterial(aiMaterial *aMaterial) {
        Material glMaterial(Vec3(0), 0.01, 0.01, 1.0);

        aiColor3D kd(0.f, 0.f, 0.f);
        if (aMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, kd) == 0) {
            glMaterial.Albedo = Vec3(kd.r, kd.g, kd.b);
        }

        ai_real metallic;
        if (aMaterial->Get(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_METALLIC_FACTOR, metallic) == 0) {
            glMaterial.Metallic = metallic;
        }

        ai_real roughness;
        if (aMaterial->Get(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_ROUGHNESS_FACTOR, roughness) == 0) {
            glMaterial.Roughness = roughness;
        }

        glMaterial.AlbedoMap = getTextureMap(aMaterial, aiTextureType_DIFFUSE);
        glMaterial.NormalMap = getTextureMap(aMaterial, aiTextureType_NORMALS);
        glMaterial.MetallicMap = getTextureMap(aMaterial, aiTextureType_METALNESS);
        glMaterial.AoMap = getTextureMap(aMaterial, aiTextureType_AMBIENT_OCCLUSION);
        glMaterial.RoughnessMap = getTextureMap(aMaterial, aiTextureType_DIFFUSE_ROUGHNESS);
        return glMaterial;
    }

    GLuint getTextureMap(aiMaterial *aMaterial, aiTextureType type) {
        int count = aMaterial->GetTextureCount(type);

        if (count > 0) {
            for (int i = 0; i < count; i++) {
                aiString name;
                aMaterial->GetTexture(type, i, &name);

                if (loadedTexMaps.find(name.C_Str()) != loadedTexMaps.end()) {
                    return loadedTexMaps.at(name.C_Str());
                }

                std::cout << name.C_Str() << std::endl;

                std::string path = directory;
                path.append(name.C_Str());
                GLuint texID = loadTexture2D(path);
                loadedTexMaps.insert({name.C_Str(), texID});
                return texID;
            }
        }

        return 0;
    }

    const GLuint loadTexture2D(std::string filename) {
        int w, h = 0;
        stbi_set_flip_vertically_on_load(1);
        unsigned char *buffer = stbi_load(filename.c_str(), &w, &h, nullptr, 4);

        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(buffer);
        return textureID;
    }
};

#endif //CG_ENGINE_MODEL_H
