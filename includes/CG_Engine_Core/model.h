#ifndef CG_ENGINE_MODEL_H
#define CG_ENGINE_MODEL_H

#include <CG_Engine_Core/mesh.h>

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
#include <CG_Engine_Core/physics/rigidbody.h>
#include "CG_Engine_Core/algo/states.h"

#define DYNAMIC                (unsigned int)1
#define CONST_INSTANCES        (unsigned int)2 //  10
#define NO_TEX                (unsigned int)4    // 100

class Scene; // forward declaration

class Model {
public:
    std::string id;

    RigidBody rb;
    Vec3 size;

    BoundTypes boundType;

    std::vector<Mesh> meshes;
    std::vector<BoundingRegion> boundingRegions;

    std::vector<RigidBody *> instances;

    unsigned int maxNoInstances;
    unsigned int currentNoInstances;

    unsigned int switches;

    Model(std::string id, BoundTypes boundType, unsigned int maxNoInstances, unsigned int flags = 0) : id(id),
                                                                                                       boundType(
                                                                                                               boundType),
                                                                                                       switches(flags),
                                                                                                       currentNoInstances(
                                                                                                               0),
                                                                                                       maxNoInstances(
                                                                                                               maxNoInstances) {};

    // initialize method
    virtual void init() {};

    RigidBody *generateInstance(Vec3 size, float mass, Vec3 pos) {
        if (currentNoInstances >= maxNoInstances) {
            // all slots filled
            return nullptr;
        }

        instances.push_back(new RigidBody(id, size, mass, pos));
        return instances[currentNoInstances++];
    };

    void initInstances() {

        Vec3 *posData = nullptr;
        Vec3 *sizeData = nullptr;
        GLenum usage = GL_DYNAMIC_DRAW;

        std::vector<Vec3> positions, sizes;

        if (States::isActive(&switches, CONST_INSTANCES)) {
            // set data pointers

            for (unsigned int i = 0; i < currentNoInstances; i++) {
                positions.push_back(instances[i]->pos);
                sizes.push_back(instances[i]->size);
            }

            if (positions.size() > 0) {
                posData = &positions[0];
                sizeData = &sizes[0];
            }

            usage = GL_STATIC_DRAW;
        }

        // generate positions VBO
        posVBO = BufferObject(GL_ARRAY_BUFFER);
        posVBO.generate();
        posVBO.bind();
        posVBO.setData<Vec3>(UPPER_BOUND, posData, GL_DYNAMIC_DRAW);

        // generate size VBO
        sizeVBO = BufferObject(GL_ARRAY_BUFFER);
        sizeVBO.generate();
        sizeVBO.bind();
        sizeVBO.setData<Vec3>(UPPER_BOUND, sizeData, GL_DYNAMIC_DRAW);

        // set attribute pointers for each mesh
        for (unsigned int i = 0, size = meshes.size(); i < size; i++) {

            meshes[i].VAO.bind();

            // set vertex attrib pointers
            // positions
            posVBO.bind();
            posVBO.setAttPointer<Vec3>(3, 3, GL_FLOAT, 1, 0, 1);
            // size
            sizeVBO.bind();
            sizeVBO.setAttPointer<Vec3>(4, 3, GL_FLOAT, 1, 0, 1);

            ArrayObject::clear();
        }
    };

    void loadModel(std::string path) {
        Assimp::Importer import;
        const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            std::cout << "Could not load model at " << path << std::endl << import.GetErrorString() << std::endl;
            return;
        }

        directory = path.substr(0, path.find_last_of("/"));

        processNode(scene->mRootNode, scene);
    };

    virtual void render(Shader shader, float dt, Scene *scene, bool setModel = true) {
        if (setModel) {
            shader.setMat4("model", Mat4x4(1.0f));
        }

        if (!States::isActive(&switches, CONST_INSTANCES)) {
            // update VBO data

            std::vector<Vec3> positions, sizes;

            bool doUpdate = States::isActive(&switches, DYNAMIC);

            for (int i = 0; i < currentNoInstances; i++) {
                if (doUpdate) {
                    // update Rigid Body
                    instances[i]->update(dt);
                    States::activate(&instances[i]->state, INSTANCE_MOVED);
                } else {
                    States::deactivate(&instances[i]->state, INSTANCE_MOVED);
                }

                positions.push_back(instances[i]->pos);
                sizes.push_back(instances[i]->size);
            }

            posVBO.bind();
            posVBO.updateData<Vec3>(0, currentNoInstances, &positions[0]);

            sizeVBO.bind();
            sizeVBO.updateData<Vec3>(0, currentNoInstances, &sizes[0]);
        }

        shader.setFloat("material.shininess", 0.5f);

        for (unsigned int i = 0, noMeshes = meshes.size(); i < noMeshes; i++) {
            meshes[i].render(shader, currentNoInstances);
        }
    };

    void cleanup() {
        for (unsigned int i = 0; i < meshes.size(); i++) {
            meshes[i].cleanup();
        }

        posVBO.cleanup();
        sizeVBO.cleanup();
    };

    void removeInstance(unsigned int idx) {
        instances.erase(instances.begin() + idx);
        currentNoInstances--;
    };

    void removeInstance(std::string instanceId) {
        int idx = getIdx(instanceId);
        if (idx != -1) {
            instances.erase(instances.begin() + idx);
            currentNoInstances--;
        }
    };

    unsigned int getIdx(std::string id) {
        for (int i = 0; i < currentNoInstances; i++) {
            if (instances[i]->instanceId == id) {
                return i;
            }
        }
        return -1;
    };

protected:
    bool noTex;

    std::string directory;

    std::vector<Texture> textures_loaded;

    void processNode(aiNode *node, const aiScene *scene) {
        // process all meshes
        for (unsigned int i = 0; i < node->mNumMeshes; i++) {
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            Mesh newMesh = processMesh(mesh, scene);
            meshes.push_back(newMesh);
            boundingRegions.push_back(newMesh.br);
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
            br.ogMin = min;
            br.max = max;
            br.ogMax = max;
        } else {
            // calculate max distance from the center
            br.center = BoundingRegion(min, max).calculateCenter();
            br.ogCenter = br.center;
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

            br.radius = sqrt(maxRadiusSquared);
            br.ogRadius = br.radius;
        }

        // process indices
        for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];
            for (unsigned int j = 0; j < face.mNumIndices; j++) {
                indices.push_back(face.mIndices[j]);
            }
        }

        Mesh ret;

        // process material
        if (mesh->mMaterialIndex >= 0) {
            aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

            if (States::isActive<unsigned int>(&switches, NO_TEX)) {
                // 1. diffuse colors
                aiColor4D diff(1.0f);
                aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diff);
                // 2. specular colors
                aiColor4D spec(1.0f);
                aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &spec);

                ret = Mesh(br, diff, spec);
            } else {
                // 1. diffuse maps
                std::vector<Texture> diffuseMaps = loadTextures(material, aiTextureType_DIFFUSE);
                textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
                // 2. specular maps
                std::vector<Texture> specularMaps = loadTextures(material, aiTextureType_SPECULAR);
                textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

                ret = Mesh(br, textures);
            }
        }

        ret.loadData(vertices, indices);
        return ret;
    };

    std::vector<Texture> loadTextures(aiMaterial *mat, aiTextureType type) {
        std::vector<Texture> textures;

        for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
            aiString str;
            mat->GetTexture(type, i, &str);

            // prevent duplicate loading
            bool skip = false;
            for (unsigned int j = 0; j < textures_loaded.size(); j++) {
                if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0) {
                    textures.push_back(textures_loaded[j]);
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

    // VBOs for positions and sizes
    BufferObject posVBO;
    BufferObject sizeVBO;
};

#endif //CG_ENGINE_MODEL_H
