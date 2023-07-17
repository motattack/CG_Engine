#ifndef CG_ENGINE_SOURCE_H
#define CG_ENGINE_SOURCE_H

#include <map>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <stdexcept>
#include <sstream>
#include <fstream>
#include <iostream>
#include <memory>

#include <CG_Engine/objects/mesh.h>
#include <CG_Engine/objects/model.h>
#include <CG_Engine/timer.h>

const std::string ICON_PATH = "res/gui/";
const std::string MODEL_PATH = "res/models/";
const std::string SHADERS_PATH = "res/shader/";
const std::string TEXTURE_PATH = "res/textures/";

class Source {
private:
    std::map<std::string, Mesh> meshes;
    std::map<std::string, GLuint> shaders;
    std::map<std::string, GLuint> cubemaps;
    std::map<std::string, GLuint> texture2ds;
    std::map<std::string, std::shared_ptr<Model>> models;

    Source() {};

    void createSphere(GLuint X_SEGMENTS = 64, GLuint Y_SEGMENTS = 64) {
        std::vector<GLuint> indices;
        std::vector<Vertex> vertices;
        const float PI = 3.14159265359;

        for (unsigned int y = 0; y <= Y_SEGMENTS; ++y) {
            for (unsigned int x = 0; x <= X_SEGMENTS; ++x) {
                float xSegment = (float) x / (float) X_SEGMENTS;
                float ySegment = (float) y / (float) Y_SEGMENTS;
                float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
                float yPos = std::cos(ySegment * PI);
                float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

                Vertex vertex(Vec3(xPos, yPos, zPos), Vec3(xPos, yPos, zPos), Vec2(xSegment, ySegment));
                vertices.push_back(vertex);
            }
        }

        bool oddRow = false;
        for (unsigned int y = 0; y < Y_SEGMENTS; ++y) {
            if (!oddRow) { // even rows: y == 0, y == 2; and so on
                for (unsigned int x = 0; x <= X_SEGMENTS; ++x) {
                    indices.push_back(y * (X_SEGMENTS + 1) + x);
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                }
            } else {
                for (int x = X_SEGMENTS; x >= 0; --x) {
                    indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
                    indices.push_back(y * (X_SEGMENTS + 1) + x);
                }
            }
            oddRow = !oddRow;
        }

        meshes.insert({"SPHERE", Mesh(vertices, indices)});
    };

    std::string loadShaderSource(std::string filename) {
        std::ifstream shaderFileStream;
        shaderFileStream.exceptions(std::ifstream::failbit | std::fstream::badbit);

        try {
            shaderFileStream.open(filename);
            std::stringstream stringStream;
            stringStream << shaderFileStream.rdbuf();
            shaderFileStream.close();
            return stringStream.str();
        }
        catch (const std::exception &) {
            std::cerr << "Shader source not load! " << filename << std::endl;
        }
        return {};
    };

    const GLuint compileShaderSource(GLenum type, const GLchar *src) {
        GLint status;
        GLchar info_log[512];
        const GLuint shaderID = glCreateShader(type);
        glShaderSource(shaderID, 1, &src, nullptr);
        glCompileShader(shaderID);

        glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
        if (!status) {
            glGetShaderInfoLog(shaderID, 512, nullptr, info_log);
            std::cerr << "Shader compile error: " << info_log << "\n" << src << std::endl;
        }
        return shaderID;
    };

public:
    ~Source() {
        for (auto texture: texture2ds) {
            glDeleteTextures(1, &texture.second);
        }

        // destroy all shaders
        for (auto program: shaders) {
            glDeleteProgram(program.second);
        }

        shaders.clear();
        texture2ds.clear();
    };

    Source(const Source &) = delete;

    Source &operator=(const Source &) = delete;

    static Source &Ref() {
        static Source reference;
        return reference;
    }

    void init() {
        // meshes
        loadMeshes();

        // models
        loadModel("PACK", "res/models/amc/scene.gltf");
        loadModel("SPHERE", "res/models/sphere/untitled.obj");

        // shaders
        createProgram("MAIN", "vShader.glsl", "fShader.glsl");
        createProgram("SKYBOX", "sbox.vert", "sbox.frag");
        createProgram("LIGHT", "lCube.vert", "lCube.frag");

        // skybox cubeMap
        std::vector<std::string> face_paths;
        face_paths.emplace_back("res/textures/skybox/earth/right.jpg");
        face_paths.emplace_back("res/textures/skybox/earth/left.jpg");
        face_paths.emplace_back("res/textures/skybox/earth/top.jpg");
        face_paths.emplace_back("res/textures/skybox/earth/bottom.jpg");
        face_paths.emplace_back("res/textures/skybox/earth/front.jpg");
        face_paths.emplace_back("res/textures/skybox/earth/back.jpg");
        loadCubeMap("SKYBOX", face_paths);
    };

    // texture
    const GLuint loadTexture2D(std::string filename) {
        int w, h = 0;
        stbi_set_flip_vertically_on_load(1);
        unsigned char *buffer = stbi_load(filename.c_str(), &w, &h, nullptr, 4);

        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
        stbi_image_free(buffer);
        return textureID;
    };

    void addTexture2D(std::string name, const GLuint texID) {
        if (texture2ds.find(name) != texture2ds.end())
            std::cout << "Trying to add same texture twice!" << std::endl;
        texture2ds.insert({name, texID});
    };

    // Cubemap texture
    void loadCubeMap(std::string name, std::vector<std::string> faces) {
        GLuint textureID = 0;
        stbi_set_flip_vertically_on_load(0);
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        for (GLuint i = 0; i < faces.size(); i++) {
            int w, h = 0;
            unsigned char *buffer = stbi_load(faces[i].c_str(), &w, &h, nullptr, 4);
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA8, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
            stbi_image_free(buffer);
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
        cubemaps.insert({name, textureID});
    };

    const GLuint cubeMap(std::string name) {
        assert(cubemaps.find(name) != cubemaps.end() && "Cubemap out of range!");
        return cubemaps.at(name);
    }

    // meshes
    void loadMeshes() {
        std::vector<Vertex> cubeVertices = {
                // POSITION					       // NORMALS					 // TEXTURE
                Vertex(Vec3(-0.5f, -0.5f, -0.5f), Vec3(0.0f, 0.0f, -1.0f), Vec2(0.0f, 0.0f)),
                Vertex(Vec3(0.5f, -0.5f, -0.5f), Vec3(0.0f, 0.0f, -1.0f), Vec2(1.0f, 0.0f)),
                Vertex(Vec3(0.5f, 0.5f, -0.5f), Vec3(0.0f, 0.0f, -1.0f), Vec2(1.0f, 1.0f)),
                Vertex(Vec3(0.5f, 0.5f, -0.5f), Vec3(0.0f, 0.0f, -1.0f), Vec2(1.0f, 1.0f)),
                Vertex(Vec3(-0.5f, 0.5f, -0.5f), Vec3(0.0f, 0.0f, -1.0f), Vec2(0.0f, 1.0f)),
                Vertex(Vec3(-0.5f, -0.5f, -0.5f), Vec3(0.0f, 0.0f, -1.0f), Vec2(0.0f, 0.0f)),

                Vertex(Vec3(-0.5f, -0.5f, 0.5f), Vec3(0.0f, 0.0f, 1.0f), Vec2(0.0f, 0.0f)),
                Vertex(Vec3(0.5f, -0.5f, 0.5f), Vec3(0.0f, 0.0f, 1.0f), Vec2(1.0f, 0.0f)),
                Vertex(Vec3(0.5f, 0.5f, 0.5f), Vec3(0.0f, 0.0f, 1.0f), Vec2(1.0f, 1.0f)),
                Vertex(Vec3(0.5f, 0.5f, 0.5f), Vec3(0.0f, 0.0f, 1.0f), Vec2(1.0f, 1.0f)),
                Vertex(Vec3(-0.5f, 0.5f, 0.5f), Vec3(0.0f, 0.0f, 1.0f), Vec2(0.0f, 1.0f)),
                Vertex(Vec3(-0.5f, -0.5f, 0.5f), Vec3(0.0f, 0.0f, 1.0f), Vec2(0.0f, 0.0f)),

                Vertex(Vec3(-0.5f, 0.5f, 0.5f), Vec3(-1.0f, 0.0f, 0.0f), Vec2(1.0f, 0.0f)),
                Vertex(Vec3(-0.5f, 0.5f, -0.5f), Vec3(-1.0f, 0.0f, 0.0f), Vec2(1.0f, 1.0f)),
                Vertex(Vec3(-0.5f, -0.5f, -0.5f), Vec3(-1.0f, 0.0f, 0.0f), Vec2(0.0f, 1.0f)),
                Vertex(Vec3(-0.5f, -0.5f, -0.5f), Vec3(-1.0f, 0.0f, 0.0f), Vec2(0.0f, 1.0f)),
                Vertex(Vec3(-0.5f, -0.5f, 0.5f), Vec3(-1.0f, 0.0f, 0.0f), Vec2(0.0f, 0.0f)),
                Vertex(Vec3(-0.5f, 0.5f, 0.5f), Vec3(-1.0f, 0.0f, 0.0f), Vec2(1.0f, 0.0f)),

                Vertex(Vec3(0.5f, 0.5f, 0.5f), Vec3(1.0f, 0.0f, 0.0f), Vec2(1.0f, 0.0f)),
                Vertex(Vec3(0.5f, 0.5f, -0.5f), Vec3(1.0f, 0.0f, 0.0f), Vec2(1.0f, 1.0f)),
                Vertex(Vec3(0.5f, -0.5f, -0.5f), Vec3(1.0f, 0.0f, 0.0f), Vec2(0.0f, 1.0f)),
                Vertex(Vec3(0.5f, -0.5f, -0.5f), Vec3(1.0f, 0.0f, 0.0f), Vec2(0.0f, 1.0f)),
                Vertex(Vec3(0.5f, -0.5f, 0.5f), Vec3(1.0f, 0.0f, 0.0f), Vec2(0.0f, 0.0f)),
                Vertex(Vec3(0.5f, 0.5f, 0.5f), Vec3(1.0f, 0.0f, 0.0f), Vec2(1.0f, 0.0f)),

                Vertex(Vec3(-0.5f, -0.5f, -0.5f), Vec3(0.0f, -1.0f, 0.0f), Vec2(0.0f, 1.0f)),
                Vertex(Vec3(0.5f, -0.5f, -0.5f), Vec3(0.0f, -1.0f, 0.0f), Vec2(1.0f, 1.0f)),
                Vertex(Vec3(0.5f, -0.5f, 0.5f), Vec3(0.0f, -1.0f, 0.0f), Vec2(1.0f, 0.0f)),
                Vertex(Vec3(0.5f, -0.5f, 0.5f), Vec3(0.0f, -1.0f, 0.0f), Vec2(1.0f, 0.0f)),
                Vertex(Vec3(-0.5f, -0.5f, 0.5f), Vec3(0.0f, -1.0f, 0.0f), Vec2(0.0f, 0.0f)),
                Vertex(Vec3(-0.5f, -0.5f, -0.5f), Vec3(0.0f, -1.0f, 0.0f), Vec2(0.0f, 1.0f)),

                Vertex(Vec3(-0.5f, 0.5f, -0.5f), Vec3(0.0f, 1.0f, 0.0f), Vec2(0.0f, 1.0f)),
                Vertex(Vec3(0.5f, 0.5f, -0.5f), Vec3(0.0f, 1.0f, 0.0f), Vec2(1.0f, 1.0f)),
                Vertex(Vec3(0.5f, 0.5f, 0.5f), Vec3(0.0f, 1.0f, 0.0f), Vec2(1.0f, 0.0f)),
                Vertex(Vec3(0.5f, 0.5f, 0.5f), Vec3(0.0f, 1.0f, 0.0f), Vec2(1.0f, 0.0f)),
                Vertex(Vec3(-0.5f, 0.5f, 0.5f), Vec3(0.0f, 1.0f, 0.0f), Vec2(0.0f, 0.0f)),
                Vertex(Vec3(-0.5f, 0.5f, -0.5f), Vec3(0.0f, 1.0f, 0.0f), Vec2(0.0f, 1.0f)),
        };
        std::vector<unsigned int> indices(36);
        for (unsigned int i = 0; i < 36; i++) {
            indices[i] = i;
        }
        createSphere();
        meshes.insert({"CUBE", Mesh(cubeVertices, indices)});
    };

    Mesh &getMesh(std::string name) {
        assert(meshes.find(name) != meshes.end() && "Mesh out of range!");
        return meshes[name];
    }

    // models
    void loadModel(const std::string name, const std::string filename) {
        std::string new_name = name;
        if (!new_name.size()) {
            new_name = "Sample Text " + std::to_string(models.size());
        }

        // Check if the model name already exists in the map
        if (models.find(new_name) != models.end()) {
            std::cout << "Model with name \"" << new_name << "\" already exists. Skipping load." << std::endl;
            return;
        }

        if (Model::validModel(filename)) {
            models.insert({new_name, std::make_shared<Model>(filename)});
            std::cout << new_name << filename << " loaded!" << std::endl;
        }
    };

    std::map<std::string, std::shared_ptr<Model>> &getModels() {
        return models;
    }

    std::shared_ptr<Model> getModel(std::string name) {
        assert(models.find(name) != models.end() && "Model out of range!");
        return models[name];
    }

    // shaders
    const GLuint program(std::string name) {
        if (shaders.find(name) == shaders.end())
            std::cout << "Program out of range" << std::endl;
        return shaders[name];
    };

    void createProgram(std::string name, std::string vtxFile, std::string fragFile) {
        if (shaders.find(name) != shaders.end())
            std::cout << "ShaderID all ready used!" << std::endl;

        const GLuint vertexShader = compileShaderSource(GL_VERTEX_SHADER,
                                                        loadShaderSource(SHADERS_PATH + vtxFile).c_str());
        const GLuint fragmentShader = compileShaderSource(GL_FRAGMENT_SHADER,
                                                          loadShaderSource(SHADERS_PATH + fragFile).c_str());

        GLuint program = 0;
        program = glCreateProgram();
        glAttachShader(program, vertexShader);
        glAttachShader(program, fragmentShader);
        glLinkProgram(program);

        GLint status;
        GLchar info_log[512];
        glGetProgramiv(program, GL_LINK_STATUS, &status);

        if (!status) {
            glGetProgramInfoLog(program, 512, nullptr, info_log);
            std::cerr << "Shader linking error: " << info_log << std::endl;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        shaders.insert({name, program});
    };
};

static Source &Resource = Source::Ref();

#endif //CG_ENGINE_SOURCE_H
